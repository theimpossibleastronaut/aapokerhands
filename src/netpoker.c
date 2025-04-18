/*
 netpoker.c
 https://github.com/theimpossibleastronaut/aapokerhands

 MIT License

 Copyright (c) 2022 Andy Alt and James Sherratt

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
#include "net.h"

int main(int argc, char *argv[])
{
  srand (time (NULL));

  struct socket_info_t socket_info = {
    .port = default_port,
    .host = NULL,
  };
  assign_tcp_dual_stack_server_fd(&socket_info);

  struct sockaddr_storage client_addr;
  socklen_t addr_size = sizeof(client_addr);
  socket_t connfd = accept(socket_info.sockfd, (struct sockaddr *)&client_addr, &addr_size);
  if (socket_info.sockfd == INVALID_SOCKET) {
    perror("Client connection failed");
  }

  st_deck_dh deck;
  deck_init_dh (&deck);
  deck_shuffle_dh(&deck);
  int i;
  for (i = 0; i < 5; i++) {
    char buf[256] = { 0 };
    snprintf (buf, sizeof buf, "%5s of %2s",
            get_card_face (deck.card[i]), get_card_suit (deck.card[i]));

    ssize_t bytes_sent = send(connfd, buf, strlen(buf) + 1, 0);
    if (bytes_sent == -1)
      perror("send");
  }

  if (connfd != INVALID_SOCKET)
    close_socket_checked(connfd);
  return 0;
}
