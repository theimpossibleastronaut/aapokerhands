/*
 net.c
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "net.h"
#include "netpoker.pb-c.h"

const char *default_port = "61357";

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

void assign_tcp_dual_stack_server_fd(struct socket_info_t *socket_info) {

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

void assign_tcp_dual_stack_client_fd(struct socket_info_t *socket_info) {
  struct addrinfo hints, *res, *p;

  // Set up hints for getaddrinfo()
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // Allow both IPv4 and IPv6
  hints.ai_socktype = SOCK_STREAM;

  // Get address info
  if (getaddrinfo(socket_info->host, socket_info->port, &hints, &res) != 0) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  // Try to connect to one of the results
  for (p = res; p != NULL; p = p->ai_next) {
    socket_info->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (socket_info->sockfd == INVALID_SOCKET)
      continue;

    if (connect(socket_info->sockfd, p->ai_addr, p->ai_addrlen) == 0)
      break; // Connected successfully

    close_socket_checked(socket_info->sockfd);
  }

  freeaddrinfo(res);

  if (!p) {
    perror("Failed to connect");
    exit(EXIT_FAILURE);
  }
  return;
}

void close_socket_checked(socket_t sockfd) {
#ifdef _WIN32
  if (closesocket(sockfd) != 0)
    perror("closesocket");
#else
  if (close(sockfd) != 0)
    perror("close");
#endif
}

uint8_t *serialize_player(const struct player_t *src, size_t *size_out) {
  Game__Player msg = GAME__PLAYER__INIT;
  msg.name = (char *)src->name;

  // Allocate memory for repeated CardInfo
  Game__Card **cards = malloc(sizeof(Game__Card *) * HAND_SIZE);
  for (int i = 0; i < HAND_SIZE; ++i) {
    cards[i] = malloc(sizeof(Game__Card));
    game__card__init(cards[i]);
    cards[i]->face_val = src->hand[i].face_val;
    cards[i]->suit = src->hand[i].suit;
  }

  msg.n_hand = HAND_SIZE;
  msg.hand = cards;

  // Serialize
  *size_out = game__player__get_packed_size(&msg);
  uint8_t *buffer = malloc(*size_out);
  game__player__pack(&msg, buffer);

  // Cleanup protobuf dynamic stuff
  for (int i = 0; i < HAND_SIZE; ++i) {
    free(cards[i]);
  }
  free(cards);

  return buffer;
}

struct player_t deserialize_player(const uint8_t *data, size_t size) {
  struct player_t out = {0};

  Game__Player *msg = game__player__unpack(NULL, size, data);
  if (!msg) {
    fprintf(stderr, "Failed to unpack player\n");
    exit(1); // or return an error struct, or set a global error
  }

  if (msg->name) {
    strncpy(out.name, msg->name, sizeof(out.name) - 1);
  }

  for (size_t i = 0; i < msg->n_hand && i < HAND_SIZE; ++i) {
    if (msg->hand[i]) {
      out.hand[i].face_val = msg->hand[i]->face_val;
      out.hand[i].suit = msg->hand[i]->suit;
    }
  }

  game__player__free_unpacked(msg, NULL);
  return out;
}

ssize_t send_all(int sockfd, const void *buf, size_t len) {
  size_t total_sent = 0;
  const uint8_t *p = buf;

  while (total_sent < len) {
    ssize_t n = send(sockfd, p + total_sent, len - total_sent, 0);
    if (n <= 0)
      return -1; // Error or disconnect
    total_sent += n;
  }
  return total_sent;
}

int recv_all(int sock, void *buffer, size_t length) {
  size_t received = 0;
  while (received < length) {
    ssize_t ret = recv(sock, (char *)buffer + received, length - received, 0);
    if (ret <= 0)
      return -1;
    received += ret;
  }
  return 0;
}
