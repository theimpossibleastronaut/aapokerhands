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

#include "globals.h"
#include "lib.h"

bool SHOW_HAND;
bool verbose;

/* Leave set to 0, PLAY feature not finished */
bool PLAY = 0;

void init(int *hand_seq, bool *final_hand, short int *hand_suits) {
  int i;

  for (i = 0; i < ACE_HIGH; i++)
    hand_seq[i] = 0;

  for (i = 0; i < NUM_OF_SUITS; i++)
    hand_suits[i] = 0;

  /* set all array elements to 0 */
  for (i = 0; i < NUM_HAND_RANKS; i++)
    final_hand[i] = false;
}

void isStraight(int *hand_seq, bool *isHighStraight, bool *final_hand) {
  int k = 13;

  short state = 0;

  /* Copy ACES for high straight/royal flush checking */
  if (hand_seq[0] == 1)
    hand_seq[13] = 1;

  /* The hand is never sorted numerically, instead, just look for
   * 5 consecutive 1's.*/
  do {

    /* May be a baby straight, not breaking if k == 13 */
    if (hand_seq[k] == 1 && hand_seq[k - 1] == 0 && k != 13) {
      break;
    } else if (hand_seq[k] == 1 && hand_seq[k - 1] == 1) {
      state++;
      if (state == 4) {
        final_hand[STRAIGHT] = 1;
        if (k == 10) {
          /* printf("High Straight"); */
          *isHighStraight = 1;
        }
      }
    }

  } while (--k && !final_hand[STRAIGHT]);
}

bool is_flush(short int *hand_suits) {
  int i;
  for (i = 0; i < NUM_OF_SUITS; ++i) {
    if (hand_suits[i] != HAND && hand_suits[i])
      break;
    else if (hand_suits[i] == HAND)
      return true;
  }
  return false;
}

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

short int find_matches(int *hand_seq, bool *final_hand) {
  int face_val;
  short paired = 0;

  for (face_val = 0; face_val < ACE_HIGH; ++face_val) {
    if (hand_seq[face_val] > 1) {
      paired++;
    }
    if (hand_seq[face_val] == 2 && !final_hand[PAIR]) {
      final_hand[PAIR] = 1;
    } else if (hand_seq[face_val] == 2) {
      final_hand[TWO_PAIR] = 1;
      final_hand[PAIR] = 0;
    } else if (hand_seq[face_val] == 3)
      final_hand[THREE_OF_A_KIND] = 1;
    else if (hand_seq[face_val] == 4) {
      final_hand[FOUR_OF_A_KIND] = 1;
      break;
    }
    if (paired > 1)
      break;
  }
  return paired;
}

void show_totals(int *totals, int RUN_COUNT) {
  int n;

  printf("Out of %d hands:\n\n", RUN_COUNT);

  for (n = 0; n < NUM_HAND_RANKS; n++) {
    printf("%20s: %9d\n", ranks[n], totals[n]);
  }
}

void hand_eval(int *totals, bool isHighStraight, bool *final_hand) {

  short eval = -1;

  if (final_hand[PAIR] && final_hand[THREE_OF_A_KIND] != 1) {
    eval = PAIR;
    totals[PAIR]++;
  } else if (final_hand[TWO_PAIR]) {
    eval = TWO_PAIR;
    totals[TWO_PAIR]++;
  } else if (final_hand[THREE_OF_A_KIND] && final_hand[PAIR] != 1) {
    eval = THREE_OF_A_KIND;
    totals[THREE_OF_A_KIND]++;
  } else if (final_hand[STRAIGHT] && final_hand[FLUSH] != 1) {
    eval = STRAIGHT;
    totals[STRAIGHT]++;
  } else if (final_hand[FLUSH] == 1 && final_hand[STRAIGHT] != 1) {
    eval = FLUSH;
    totals[FLUSH]++;
  } else if (final_hand[PAIR] && final_hand[THREE_OF_A_KIND]) {
    /* final_hand[FULL_HOUSE] = 1; */
    eval = FULL_HOUSE;
    totals[FULL_HOUSE]++;
  } else if (final_hand[FOUR_OF_A_KIND]) {
    eval = FOUR_OF_A_KIND;
    totals[FOUR_OF_A_KIND]++;
  } else if (final_hand[STRAIGHT] && final_hand[FLUSH] && isHighStraight != 1) {
    /* final_hand[7] = 1;    */
    eval = STRAIGHT_FLUSH;
    totals[STRAIGHT_FLUSH]++;
  } else if (final_hand[STRAIGHT] && final_hand[FLUSH] && isHighStraight) {
    eval = ROYAL_FLUSH;
    totals[ROYAL_FLUSH]++;
  }

  if (SHOW_HAND) {
    printf("\n\t\t-->");
    if (eval > -1)
      printf("%s", ranks[eval]);
    else
      printf("Nothing");

    printf("<--\n\n");
  }

  return;
}

void main_thread(st_deck_dh *deck, const int RUN_COUNT, int *totals) {

  int run_count = 0;

  /* Start main program loop */
  while (run_count++ < RUN_COUNT) {
    int i, j, k;
    i = j = k = 0;

    deck_shuffle_dh(deck);

    bool final_hand[NUM_HAND_RANKS];
    int hand_seq[ACE_HIGH];
    short int hand_suits[NUM_OF_SUITS];
    init(hand_seq, final_hand, hand_suits);

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

    /* +4 for the unimplemented PLAY feature */
    st_hand hand;

    /* Deal out a hand */

    do {
      if (PLAY && k < 5) {
        printf("(%d)%5s of %2s", k + 1, get_card_face(deck->card[i]), get_card_suit(deck->card[i]));
        if (++j != 4)
          printf(" | ");
        else {
          CR;
          j = 0;
        }
      }

      hand.card[k].suit = deck->card[i].suit;
      hand.card[k].face_val = deck->card[i].face_val;

      /* Deal out every other card */
      i += 2;
    } while (++k < HAND + (PLAY * 4));

    int suitn, valuen;

    if (PLAY) {
      short discard = 0;
      int next = 5;
      for (j = 5; j < 9; j++) {
        scanf("%hd", &discard);
        if (discard != 0) {
          suitn = hand.card[next].suit;
          hand.card[discard - 1].face_val = hand.card[next++].face_val;
          hand.card[discard - 1].suit = suitn;
        }
      }
      CR;
    }

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

    } while (++k < HAND);

    for (i = 0; i < HAND; i++) {

      /* if hand.card[i].face_value is 13 (King), valuen will equal 12
       * so hand_seq[12] will be incremented. If there are 3
       * Kings, hand_seq[12] will equal 3     */

      valuen = hand.card[i].face_val - 1;
      hand_seq[valuen]++;

      /* if hand.card[i].suit == 2 (Spades), then hand_suits[2] will
       * be incremented. If hand_suits[2] == 5, a flush will be
       * found when is_flush() is called.    */

      hand_suits[hand.card[i].suit]++;
    }

    /* Evaluate the hand */
    short paired = find_matches(hand_seq, final_hand);

    /* if no matches were found, check for flush and straight
     * if there were any matches, flush or straight is impossible,
     * so don't bother checking               */

    bool isHighStraight = 0;

    if (!paired) {
      isStraight(hand_seq, &isHighStraight, final_hand);
      final_hand[FLUSH] = is_flush(hand_suits);
    }

    hand_eval(totals, isHighStraight, final_hand);
  }

  return;
}

int main(int argc, char *argv[]) {
  /* Number of hands to deal out */
  /* can be changed from the command line with -n [hands] */
  int RUN_COUNT = 20;
  verbose = 0;
  SHOW_HAND = 0;
  PLAY = 0;

  getopts(argc, argv, &RUN_COUNT);

  if (RUN_COUNT > INT_MAX - 1) {
    printf("Your RUN_COUNT should be lower than INT_MAX\n");
    return 1;
  }

  if (PLAY && verbose) {
    verbose = 0;
  }

  if (PLAY && SHOW_HAND) {
    SHOW_HAND = 1;
  }

  int totals[9];

  int i;
  for (i = 0; i < NUM_HAND_RANKS; i++)
    totals[i] = 0;

  st_deck_dh deck;
  deck_init_dh(&deck);

  /* seeding the random number generator, used by deck_shuffle_dh() */
  srand(time(NULL));

  main_thread(&deck, RUN_COUNT, totals);

  /* Show totals for all hands */
  show_totals(totals, RUN_COUNT);

  /* print a newline before the program ends */
  CR;
  return 0;
}
