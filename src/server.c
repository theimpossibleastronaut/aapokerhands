/*
 server.c
 https://github.com/theimpossibleastronaut/aapokerhands

 MIT License

 Copyright (c) 2025 Andy Alt

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#include <deckhandler.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "lib.h"
#include "net.h"
#include "server.h"
#include "types.h"
#include "netpoker.pb-c.h"

static void set_sock_reuse(socket_t sockfd) {
  int r = -1;
#ifdef _WIN32
  // When starting the server immediately after it was killed,
  // prevent the error "Address already in use" when running
  // See https://linux.die.net/man/3/setsockopt for more information.
  const char opt = 1;
  r = (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0);

#else
  int opt = 1;
  r = (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0);

#endif
  if (r == -1)
    perror("setsockopt SO_REUSEADDR");
}

static void set_sock_ipv6_v6only_disable(socket_t sockfd, const int ai_family) {
  int r = -1;
#ifdef _WIN32
  if (ai_family == AF_INET6) {
    const char optval = 0;
    r = (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval)) < 0);
  }
#else
  (void)ai_family;
  int optval = 0;
  r = (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval)) < 0);
#endif
  if (r == -1)
    perror("setsockopt IPV6_V6ONLY");
}

static void assign_tcp_dual_stack_server_fd(struct socket_info_t *socket_info) {

  struct addrinfo hints, *res, *p;

  // Set up hints for dual-stack
  memset(&hints, 0, sizeof(hints));
  // Use IPv6, but allow IPv4 via v6-mapped addresses
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // Auto-fill IP

  // Get address info
  if (getaddrinfo(socket_info->host, socket_info->port, &hints, &res) != 0) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  // Create and bind socket
  for (p = res; p != NULL; p = p->ai_next) {
    socket_info->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (socket_info->sockfd == INVALID_SOCKET)
      continue;

    set_sock_reuse(socket_info->sockfd);
    set_sock_ipv6_v6only_disable(socket_info->sockfd, p->ai_family);

    if (bind(socket_info->sockfd, p->ai_addr, p->ai_addrlen) == 0)
      break; // Success
    close_socket_checked(socket_info->sockfd);
  }

  freeaddrinfo(res);
  if (!p) {
    perror("Failed to bind");
    exit(EXIT_FAILURE);
  }

  // Start listening
  if (listen(socket_info->sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %s...\n", socket_info->port);
  return;
}

struct accept_args_t {
  struct socket_info_t *socket_info;
  struct player_t *player;
  struct dh_deck *deck;
};

static void *accept_thread(void *arg) {
  struct accept_args_t *args = (struct accept_args_t *)arg;
  while (1) {
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(client_addr);
    socket_t connfd =
        accept(args->socket_info->sockfd, (struct sockaddr *)&client_addr, &addr_size);
    if (connfd == INVALID_SOCKET) {
      perror("Client connection failed");
    } else
      puts("Connection established");

    for (int i = 0; i < 5; i++) {
      args->player->hand.card[i].face_val = args->deck->card[i].face_val;
      args->player->hand.card[i].suit = args->deck->card[i].suit;
    }

    size_t size = 0;
    uint8_t *data = serialize_player(args->player, &size);
    printf("size: %zd\n", size);

    uint32_t size_net = htonl(size); // Ensure network byte order
    printf("size_net: %d\n", size_net);
    if (send_all(connfd, &size_net, sizeof(size_net)) != sizeof(size_net)) {
      perror("send size");
    }

    // Send the serialized data
    if (send_all(connfd, data, size) == -1)
      perror("send");

    free(data);

    if (connfd != INVALID_SOCKET)
      close_socket_checked(connfd);
  }

  return NULL;
}

static void init_players(struct player_t *player) {
  const struct preset_player_pos_t preset_player_pos = {
      .pos = {
          // P0: bottom center
          {.x = WINDOW_WIDTH / 3, .y = WINDOW_HEIGHT - 80},

          // P1: top-left
          {.x = 20, .y = 20},

          // P2: left, 1/3 down
          {.x = 20, .y = WINDOW_HEIGHT / 3},

          // P3: top-right
          {.x = WINDOW_WIDTH - 70, .y = 20},

          // P4: right, 1/3 down
          {.x = WINDOW_WIDTH - 70, .y = WINDOW_HEIGHT / 3},
      }};

  // This offers only a little extra protection if changes are made.
  _Static_assert(sizeof(preset_player_pos.pos) / sizeof(preset_player_pos.pos[0]) == 5,
                 "preset_player_pos.pos has wrong number of elements");

  for (int i = 0; i < MAX_PLAYERS; i++) {
    player[i] = (struct player_t){
        .name = "Testy", .id = -1, .pos = preset_player_pos.pos[i], .chips = 20000};
  }
}

int run_server(void) {

  struct player_t player[MAX_PLAYERS];
  init_players(player);

  dh_pcg_srand_auto();

#ifdef _WIN32
  WSADATA wsaData;
  int iResult;

  // Initialize Winsock version 2.2
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  }
#endif

  struct socket_info_t socket_info = {
      .port = default_port,
      .host = NULL,
  };
  assign_tcp_dual_stack_server_fd(&socket_info);

  // struct accept_args_t {
  // socket_t sockfd;
  //} accept_args = { .sockfd =

  struct dh_deck deck;
  dh_init_deck(&deck);
  dh_shuffle_deck(&deck);

  struct accept_args_t accept_args = {
      .socket_info = &socket_info,
      .player = &player[0],
      .deck = &deck,
  };

  pthread_t accepter;
  pthread_create(&accepter, NULL, accept_thread, &accept_args);
  pthread_join(accepter, NULL);

#ifdef _WIN32
  WSACleanup();
#endif

  return 0;
}
