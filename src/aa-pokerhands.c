/*
 * aa-pokerhands.c
 * This file is part of aa-pokerhands
 * <https://github.com/theimpossibleastronaut/aa-pokerhands>
 *
 * Copyright 2011-2018 Andy <andy400-dev@yahoo.com>
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

#include "aa-pokerhands.h"
#include "functions.h"

int main(int argc, char *argv[]) {

  RUN_COUNT = 20;
  MORE_OUTPUT = 0;
  SHOW_HAND = 0;
  PLAY = 0;

  char *suits[] = {
  "Hearts  ",
  "Diamonds",
  "Spades  ",
  "Clubs   "
  };

  char *faces[] = {
  "Ace",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "10",
  "Jack",
  "Queen",
  "King"
  };

  high_straight = 0;

  srand( time ( 0 ) );

  register loop i, j, k;

    register short suitn, valuen;

    short discard, next;

    short paired;

    int run_count = 0;

    getopts(argc, argv);

  if (RUN_COUNT > INT_MAX - 1)
    {
      printf("Your RUN_COUNT should be lower than INT_MAX\n");
      exit(1);
    }

    if (PLAY && MORE_OUTPUT)
    {
      MORE_OUTPUT = 0;
    }

    if (PLAY && SHOW_HAND)
    {
        SHOW_HAND = 1;
    }

  /* deck will have initial values 1 - 52 */

  for (i = 0; i < 52; i++) {
    deck.values[i][0] = i + 1;
    deck.values[i][1] = 0;
  }

  make_real();

  /* Start main program loop */

    while (run_count++ < RUN_COUNT) {

    shuffle();

    if (MORE_OUTPUT) {

      i = j = 0;
      do {
        suitn = shuffled.values[i][1];
        valuen = shuffled.values[i][0] - 1;
        printf("%5s of %2s", faces[valuen], suits[suitn]);

        /* print newline every 4 cards */
        if (++j != 4)
          printf(" | ");
        else {
          CR
          j = 0;
        }


      } while (++i < 52);
      CR
    }


    if (SHOW_HAND)
      CR

    zero();

    /* Deal out a hand */

    i = j = k = 0;
    do {
      if (PLAY && k < 5) {
        suitn = shuffled.values[i][1];
        valuen = shuffled.values[i][0] - 1;
        printf("(%d)%5s of %2s",k + 1, faces[valuen], suits[suitn]);
        if (++j != 4)
          printf(" | ");
        else {
          CR
          j = 0;
        }
      }

      hand[k][0] = shuffled.values[i][0];
      hand[k][1] = shuffled.values[i][1];

      /* Deal out every other card */
      i += 2;

    }while (++k < HAND + (PLAY * 4) );

    /* CR */

    if (PLAY) {
      next = 5;
      for (j = 5; j < 9; j++){
        scanf("%hd",&discard);
        if (discard != 0) {
          suitn = hand[next][1];
          hand[discard - 1][0] = hand[next++][0];
          hand[discard - 1][1] = suitn;
        }
      }
      CR
    }

    i = j = k = 0;
    do {
      if (SHOW_HAND) {
        suitn = hand[k][1];
        valuen = hand[k][0] - 1;
        printf("%5s of %2s", faces[valuen],suits[suitn]);
        if (++j != 4)
          printf(" | ");
        else {
          CR
          j = 0;
        }
      }

      /* Deal out every other card, out of the first 10 */
      i++;

    }while (++k < HAND);



    for (i = 0; i < HAND; i++) {

      /* if hand[i][0] is 13 (King), valuen will equal 12
       * so hand_seq[12] will be incremented. If there are 3
       * Kings, hand_seq[12] will equal 3     */

      valuen = hand[i][0] - 1;
      hand_seq[valuen]++;

      /* if hand[i][1] == 2 (Spades), then hand_suits[2] will
       * be incremented. If hand_suits[2] == 5, a flush will be
       * found when isFlush() is called.    */

      suitn = hand[i][1];
      hand_suits[suitn]++;
    }

    /* Evaluate the hand */
    paired = find_matches();

    /* if no matches were found, check for flush and straight
     * if there were any matches, flush or straight is impossible,
     * so don't bother checking               */
    if ( !paired ) {
      isStraight();
      isFlush();
    }

    const char *ranks[] = {
      "Pair",
      "Two Pair",
      "Three-of-a-Kind",
      "Straight",
      "Flush",
      "Full House",
      "Four-of-a-Kind",
      "Straight Flush",
      "Royal Flush"
    };

    hand_eval( run_count, ranks );

  } /* End main program loop  */

  /* print a newline before the program ends */
  CR

  return 0;

}
