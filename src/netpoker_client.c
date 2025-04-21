/*
 netpoker_client.c
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
#include "graphics.h"
#include "net.h"
#include "netpoker.pb-c.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  struct socket_info_t socket_info = {
      .port = default_port,
      .host = "127.0.0.1",
  };
  assign_tcp_dual_stack_client_fd(&socket_info);

  uint32_t size_net = 0;
  if (recv_all(socket_info.sockfd, &size_net, sizeof(size_net)) != 0) {
    perror("recv size");
    exit(1);
  }

  printf("before conversion: %d\n", size_net);
  size_t size = ntohl(size_net);
  printf("after conversion: %zd\n", size);

  uint8_t *data = malloc(size);
  if (!data) {
    perror("malloc");
    exit(1);
  }

  ssize_t received = recv(socket_info.sockfd, data, size, 0);
  if (received <= 0) {
    perror("recv");
    free(data);
    exit(1);
  } else
    puts("received data");

  // Deserialize
  struct player_t player = deserialize_player(data, size);

  printf("Deserialized name: %s\n", player.name);
  int i;
  for (i = 0; i < HAND_SIZE; ++i) {
    printf("Card %d: face=%d, suit=%d\n", i + 1, player.hand.card[i].face_val,
           player.hand.card[i].suit);
  }

  free(data);

  if (socket_info.sockfd != INVALID_SOCKET)
    close_socket_checked(socket_info.sockfd);

  struct sdl_context_t sdl_context;
  init_sdl_window(&sdl_context, "Net Poker");
  run_sdl_loop(sdl_context.renderer);
  do_sdl_cleanup(&sdl_context);

  return 0;
}
