/*
 * aapokerhands.c
 * This file is part of aapokerhands
 * <https://github.com/theimpossibleastronaut/aapokerhands>

 MIT License

 Copyright (c) 2011-2025 Andy Alt

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
#include <limits.h>
#include <pokeval.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#define DEFAULT_RUN_COUNT 100000

bool SHOW_HAND = 0;
bool verbose = 0;

#define CR printf("\n")

static void usage(const char *argv_one) {
  printf("Usage: %s: -n [hands] -s -v -V\n", argv_one);
  printf("\n\
-n [hands]          number of hands to deal\n\
-s                  show the hands that were dealt\n\
-v                  show the deck for each hand\n\
-V                  display Version information\n\
");

  exit(EXIT_FAILURE);
}

void getopts(int argc, char *argv[], int *RUN_COUNT) {
  /* fetch command line arguments */
  int i;
  if (argc > 1) {
    for (i = 1; i < argc; i++)
      if (argv[i][0] == '-')
        switch (argv[i][1]) {
        case 'n':
          if (i < argc - 1) {
            *RUN_COUNT = atoi(argv[i++ + 1]);
            if (*RUN_COUNT < 1)
              usage(argv[0]);
          } else
            usage(argv[0]);
          break;
        case 'v':
          verbose = 1;
          break;
        case 's':
          SHOW_HAND = 1;
          break;
        case 'V':
          printf("%s %s\n", PACKAGE_STRING, VERSION);
          puts(PACKAGE_URL);
          putchar('\n');
          break;
        default:
          usage(argv[0]);
          break;
        }
  }

  else
    *RUN_COUNT = DEFAULT_RUN_COUNT;
}

void show_totals(int *totals, int RUN_COUNT) {
  int n;

  printf("Out of %d hands:\n\n", RUN_COUNT);

  for (n = PAIR; n < NUM_HAND_RANKS; n++) {
    printf("%20s: %9d\n", ranks[n], totals[n]);
  }
}

void main_thread(struct dh_deck *deck, const int RUN_COUNT, int *totals) {

  int run_count = 0;

  /* Start main program loop */
  while (run_count++ < RUN_COUNT) {
    int i, j, k;
    i = j = k = 0;

    dh_shuffle_deck(deck);

    if (verbose) {
      int copy_i = i;
      int copy_j = j;
      do {
        printf("%5s of %2s", get_card_face(deck->card[copy_i]), get_card_suit(deck->card[copy_i]));

        /* print newline every 4 cards */
        if (++copy_j != 4)
          printf(" | ");
        else {
          CR;
          copy_j = 0;
        }
      } while (++copy_i < 52);
      CR;
    }

    if (SHOW_HAND)
      CR;

    struct hand_t hand;

    /* Deal out a hand */

    do {
      hand.card[k].suit = deck->card[i].suit;
      hand.card[k].face_val = deck->card[i].face_val;

      i++;
    } while (++k < HAND_SIZE);

    i = j = k = 0;
    do {
      if (SHOW_HAND) {
        printf("%5s of %2s", get_card_face(deck->card[i]), get_card_suit(deck->card[i]));
        if (++j != 4)
          printf(" | ");
        else {
          CR;
          j = 0;
        }
      }

      /* Deal out every other card, out of the first 10 */
      i++;

    } while (++k < HAND_SIZE);

    short rank = evaluate_hand(hand);
    if (rank != NOTHING)
      totals[rank]++;
    if (SHOW_HAND) {
      printf("\n\t\t-->");
      printf("%s", ranks[rank]);
      printf("<--\n\n");
    }
  }

  return;
}

int main(int argc, char *argv[]) {
  /* Number of hands to deal out */
  /* can be changed from the command line with -n [hands] */
  int RUN_COUNT = 20;
  verbose = 0;
  SHOW_HAND = 0;

  getopts(argc, argv, &RUN_COUNT);

  if (RUN_COUNT > INT_MAX - 1) {
    printf("Your RUN_COUNT should be lower than INT_MAX\n");
    return 1;
  }

  int totals[NUM_HAND_RANKS] = {0};

  printf("Evaluating %d hands...\n", RUN_COUNT);

  struct dh_deck deck;
  dh_init_deck(&deck);
  dh_pcg_srand_auto();

  main_thread(&deck, RUN_COUNT, totals);

  /* Show totals for all hands */
  show_totals(totals, RUN_COUNT);

  /* print a newline before the program ends */
  CR;
  return 0;
}
