/*
 * aapokerhands.c
 * This file is part of aapokerhands
 * <https://github.com/theimpossibleastronaut/aapokerhands>
 *
 * Copyright 2011-2023 Andy Alt <arch_stanton5995@proton.me>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "deckhandler.h"
#include "globals.h"
#include "lib.h"

bool SHOW_HAND;
bool verbose;

static void usage(const char *argv_one) {
  printf("Usage: %s: -n [hands] -s -v -V\n", argv_one);
  printf("\n\
-n [hands]          number of hands to deal\n\
-s                  show the hands that were dealt\n\
-v                  show the deck for each hand\n\
-V                  display Version information\n\
");

  exit(1);
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
    usage(argv[0]);
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

  int totals[NUM_HAND_RANKS] = { 0 };

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
