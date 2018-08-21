/*
 * aa-pokerhands.c
 *
 * Copyright 2011-2017 Andy <andy400-dev@yahoo.com>
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

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

/* cards.c   .03pre 03-04-2011  Andy Alt  */

/* Number of hands to deal out */
/* can be changed from the command line with -n [hands] */
int RUN_COUNT = 20;

/* can be set on the command line with -v */
/* Show all 52 cards after the deck has been shuffled */
bool MORE_OUTPUT = 0;

/* can be set on the command line with -s */
/* Show the hand that was dealt */
bool SHOW_HAND = 0;

/* Leave set to 0, PLAY feature not finished */
bool PLAY = 0;

enum { HEARTS, DIAMONDS, SPADES, CLUBS };
#define SUITS 4

enum { PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT, FLUSH,
       FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH };
#define RANKS 9

#define ACE 1
#define ACE_HIGH 14

#define HAND 5

#define CR printf("\n");

void getopts(int argc, char *argv[]);
void shuffle(void);
void zero(void);
void make_real(void);
short find_matches(void);
void isStraight(void);
void isFlush(void);
void usage(const char *argv_one);
void hand_eval(int run_count);
void show_totals(int *totals, int run_count);


struct card_info {
  short int values[52][2];
} deck, shuffled ;


short hand[HAND + 4][2];
short int hand_seq[ACE_HIGH];
short int hand_suits[SUITS];

const char *suits[] = {
  "Hearts  ",
  "Diamonds",
  "Spades  ",
  "Clubs   "
};

const char *faces[] = {
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
  "King",
};

const char *ranks[] = {
  "Pair",
    "Two Pair",
  "Three-of-a-Kind",
  "Straight",
  "Flush",
  "Full House",
  "Four-of-a-Kind",
  "Straight Flush",
  "Royal Flush",
};

bool final[RANKS];

bool high_straight = 0;

typedef short int loop;

int main(int argc, char *argv[]) {

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


    hand_eval( run_count );

  } /* End main program loop  */

  /* print a newline before the program ends */
  CR

  return 0;

}

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
