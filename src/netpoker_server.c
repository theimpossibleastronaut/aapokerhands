/*
 netpoker_server.c
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
#include <time.h>

#include "deckhandler.h"
#include "lib.h"
#include "net.h"
#include "netpoker.h"
#include "netpoker.pb-c.h"

// static void accept_thread(void *args) {
// while (1) {
// struct sockaddr_storage client_addr;
// socklen_t addr_size = sizeof(client_addr);
// socket_t connfd = accept(socket_info.sockfd, (struct sockaddr *)&client_addr, &addr_size);
// if (socket_info.sockfd == INVALID_SOCKET) {
// perror("Client connection failed");
//} else
// puts("Connection established");
//};
// return;
//}

static void init_players(struct player_t *player) {
  const struct preset_player_pos_t preset_player_pos = {
      .pos = {
          // P0: bottom center
          {.x = WINDOW_WIDTH / 2 - 25, .y = WINDOW_HEIGHT - 80},

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

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  struct player_t player[MAX_PLAYERS];
  init_players(player);

  srand(time(NULL));

  #ifdef _WIN32_
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

  struct sockaddr_storage client_addr;
  socklen_t addr_size = sizeof(client_addr);
  socket_t connfd = accept(socket_info.sockfd, (struct sockaddr *)&client_addr, &addr_size);
  if (socket_info.sockfd == INVALID_SOCKET) {
    perror("Client connection failed");
  } else
    puts("Connection established");

  struct dh_deck deck;
  dh_init_deck(&deck);
  dh_shuffle_deck(&deck);

  int i;
  for (i = 0; i < 5; i++) {
    player[0].hand.card[i].face_val = deck.card[i].face_val;
    player[0].hand.card[i].suit = deck.card[i].suit;
  }

  bool final_hand[NUM_HAND_RANKS];
  int hand_seq[ACE_HIGH];
  short int hand_suits[MAX_SUITS];
  init(hand_seq, final_hand, hand_suits);

  size_t size = 0;
  uint8_t *data = serialize_player(&player[0], &size);
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

  #ifdef _WIN32_
  WSACleanup();
  #endif

  return 0;
}