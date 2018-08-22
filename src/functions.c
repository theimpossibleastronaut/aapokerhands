/*
 * functions.c
 *
 * Copyright 2011-2018 Andy <andy400-dev@yahoo.com>
 * This file is part of aa-pokerhands
 * <https://github.com/theimpossibleastronaut/aa-pokerhands>
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

#ifndef INC_POKERHANDS_H
  #define INC_POKERHANDS_H
  #include "aa-pokerhands.h"
#endif

#include "aa-pokerhands.h"

void shuffle(void)  {

  /* If program is run twice in the same second, the output deck
     will be the same. Sleep for 1 second to prevent that from
     happening.             */
  /*  sleep(1);  */
  loop j;
  short card = 0;

    bool status[52];

    for (j = 0; j < 52; j++)
    status[j] = 1;

  j = 0;

  while ( j < 52 ) {
    card = rand() % 52;

    if ( status[card] ) {
      shuffled.values[j][0] = deck.values[card][0];
      shuffled.values[j][1] = deck.values[card][1];
      status[card] = 0;
      j++;
    }
  }

  /* No need to store the values unless RUN_COUNT > 1 */
  if (RUN_COUNT > 1)
    for ( j = 0; j < 52; j++ ) {
      deck.values[j][0] = shuffled.values[j][0];
      deck.values[j][1] = shuffled.values[j][1];
    }
}


void zero(void) {
  loop i, j;

  high_straight = 0;
    /* reset all elements to 0      */
  for (i = 0; i < HAND; i++)
    for (j = 0; j < 2; j++)
      hand[i][j] = 0;

  for (i = 0; i < ACE_HIGH; ++i)
    hand_seq[i] = 0;

  for (i = 0; i < SUITS; ++i)
    hand_suits[i] = 0;

     /* set all array elements to 0 */
  for (i = 0; i < RANKS; ++i)
    final[i] = 0;
}

void make_real(void) {
  loop i;
  short suitn;

  for (i = 0; i < 52; ++i) {
    if (deck.values[i][0] > 39) {
      deck.values[i][0] -= 39;
      suitn = CLUBS;
    }
    else if (deck.values[i][0] > 26) {
      deck.values[i][0] -= 26;
      suitn = SPADES;
    }
    else if (deck.values[i][0] > 13) {
      deck.values[i][0] -= 13;
      suitn = DIAMONDS;
    }
    else {
      suitn = HEARTS;
    }

    deck.values[i][1] = suitn;

    }
}

void isStraight(void) {


  loop k = 13;

  short state = 0;

  /* Copy ACES for high straight/royal flush checking */
  if (hand_seq[0] == 1)
    hand_seq[13] = 1;


    /* The hand is never sorted numerically, instead, just look for
     * 5 consecutive 1's.*/
  do {

    /* May be a baby straight, not breaking if k == 13 */
    if ( hand_seq[k] == 1 && hand_seq[k - 1] == 0 && k != 13 ) {
      break;
    }
    else if ( hand_seq[k] == 1 && hand_seq[k - 1] == 1 ) {
      state++;
      if ( state == 4 ) {
        final[STRAIGHT] = 1;
          if (k == 10) {
            /* printf("High Straight"); */
            high_straight = 1;
          }
      }
    }

    } while ( --k && ! final[STRAIGHT] );
}

void isFlush(void) {
  loop i;
  for ( i = 0; i < SUITS; ++i ) {
    if ( hand_suits[i] != HAND && hand_suits[i] )
      break;
    else if ( hand_suits[i] == HAND ) {
      final[FLUSH] = 1;
      break;
    }
  }
}



void getopts (int argc, char *argv[]) {
  /* fetch command line arguments */
  loop i;
  if (argc > 1)
  {
    for (i = 1; i < argc; i++)
      if (argv[i][0] == '-')
        switch (argv[i][1]) {
        case 'n':
            if (i < argc - 1) {
        RUN_COUNT = atoi(argv[i++ + 1]);
        if (RUN_COUNT < 1)
          usage(argv[0]);
      }
      else usage(argv[0]);
          break;
        case 'v':
            MORE_OUTPUT = 1;
          break;
        case 's':
            SHOW_HAND = 1;
          break;
        /* case 'p':
      PLAY = 1;
      break; */
        default:
          usage(argv[0]);
            break;

          }
  }

  else usage(argv[0]);
}

short int find_matches(void) {
  loop i;
  short paired = 0;

  for (i = 0; i < ACE_HIGH; ++i) {
    if ( hand_seq[i] > 1 ) {
      paired++;
    }
    if ( hand_seq[i] == 2 && ! final[PAIR] ) {
      final[PAIR] = 1;
    }
    else if (hand_seq[i] == 2) {
      final[TWO_PAIR] = 1;
      final[PAIR] = 0;
    }
    else if (hand_seq[i] == 3)
      final[THREE_OF_A_KIND] = 1;
    else if (hand_seq[i] == 4) {
      final[FOUR_OF_A_KIND] = 1;
      break;
    }
    if (paired > 1)
      break;
  }
  return paired;
}

void hand_eval(int run_count) {

  loop i;

  static int totals[RANKS];

    if (run_count == 1)
    for (i = 0; i < RANKS; i++)
      totals[i] = 0;

  short eval = -1;


  if (final[PAIR] && final[THREE_OF_A_KIND] != 1) {
    eval = PAIR;
    totals[PAIR]++;
  }
  else if (final[TWO_PAIR]) {
    eval = TWO_PAIR;
    totals[TWO_PAIR]++;
  }
  else if (final[THREE_OF_A_KIND] && final[PAIR] != 1) {
    eval = THREE_OF_A_KIND;
    totals[THREE_OF_A_KIND]++;
  }
  else if (final[STRAIGHT] && final[FLUSH] != 1) {
    eval = STRAIGHT;
    totals[STRAIGHT]++;
  }
  else if (final[FLUSH] == 1 && final[STRAIGHT] != 1) {
    eval = FLUSH;
    totals[FLUSH]++;
  }
  else if (final[PAIR] && final[THREE_OF_A_KIND]) {
    /* final[FULL_HOUSE] = 1; */
    eval = FULL_HOUSE;
    totals[FULL_HOUSE]++;
  }
  else if (final[FOUR_OF_A_KIND]) {
    eval = FOUR_OF_A_KIND;
    totals[FOUR_OF_A_KIND]++;
  }
  else if (final[STRAIGHT] && final[FLUSH] && high_straight != 1) {
    /* final[7] = 1;    */
    eval = STRAIGHT_FLUSH;
    totals[STRAIGHT_FLUSH]++;
  }
  else if (final[STRAIGHT] && final[FLUSH] && high_straight) {
    eval = ROYAL_FLUSH;
    totals[ROYAL_FLUSH]++;
  }


  if (SHOW_HAND) {
    printf("\n\t\t-->");
    if ( eval > -1 )
      printf("%s",ranks[eval]);
    else
      printf("Nothing");

    printf("<--\n\n");
  }

  /* Show totals for all hands */
  if (run_count == RUN_COUNT)
    show_totals(totals, run_count);
}


void show_totals(int *totals, int run_count) {
  loop n;

  printf("Out of %d hands:\n\n",RUN_COUNT);

  for (n = 0; n < RANKS; n++) {
    printf("%20s: %9d\n", ranks[n], totals[n]);
  }
}

void usage(const char *argv_one) {
  printf("Usage: %s: -n [hands] -s -v\n",argv_one);
  exit(1);
  }