/*
 graphics.c
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

#include "graphics.h"

void init_sdl_window(struct sdl_context_t *sdl_context, const char *title) {
  SDL_Init(SDL_INIT_VIDEO);
  const char *client = strstr(title, "Client");
  int win_pos_x = (client != NULL) ? WINDOW_WIDTH / 2 + 10 : SDL_WINDOWPOS_CENTERED;
  int win_pos_y = (client != NULL) ? WINDOW_HEIGHT / 2 + 10 : SDL_WINDOWPOS_CENTERED;
  sdl_context->window =
      SDL_CreateWindow(title, win_pos_x, win_pos_y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  sdl_context->renderer = SDL_CreateRenderer(sdl_context->window, -1, SDL_RENDERER_ACCELERATED);

  //// Draw white background
  //SDL_SetRenderDrawColor(sdl_context->renderer, 0, 125, 0, 255);
  //SDL_RenderClear(sdl_context->renderer);
  //SDL_RenderPresent(sdl_context->renderer);
  return;
}

void run_sdl_loop(SDL_Renderer *renderer) {
  int running = 1;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
      //if (event.type == SDL_MOUSEBUTTONDOWN) {
        //x = event.button.x;
        //y = event.button.y;
        //len = snprintf(message, sizeof(message), formatted_msg, x, y, shape);
        //if (send(sockfd, message, len, 0) == -1) {
          //perror("send");
        //}
      //}
    }

    // Draw background
    SDL_SetRenderDrawColor(renderer, 0, 125, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }
}

void do_sdl_cleanup(struct sdl_context_t *sdl_context) {
  SDL_DestroyRenderer(sdl_context->renderer);
  SDL_DestroyWindow(sdl_context->window);
  SDL_Quit();
}
