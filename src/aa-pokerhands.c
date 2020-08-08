/*
 * aa-pokerhands.c
 * This file is part of aa-pokerhands
 * <https://github.com/theimpossibleastronaut/aa-pokerhands>
 *
 * Copyright 2011-2020 Andy Alt <andy400-dev@yahoo.com>
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

#include <time.h>
#include <stdlib.h>
#include "aa-pokerhands.h"
#include "functions.h"
#include <pthread.h>

// FIXME: Increasing this to a value of > 1 results in an ~8x slowdown
#define num_threads 1

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

const int RANKS = ARRAY_SIZE(ranks);
bool SHOW_HAND;
bool MORE_OUTPUT;

/* Leave set to 0, PLAY feature not finished */
bool PLAY = 0;

struct thread_info {    /* Used as argument to thread_start() */
pthread_t thread_id;        /* ID returned by pthread_create() */
  int       thread_num;       /* Application-defined thread # */
  st_deck_dh *deck;
  int RUN_COUNT;
  int *totals;
};

void*
main_thread(void *arg) {
  struct thread_info *tinfo = arg;

  int run_count = 0;

  bool final_hand[RANKS];

  /* Start main program loop */
  while (run_count++ < tinfo->RUN_COUNT)
  {
    int i = 0;
    int j, k = i;

    deck_shuffle_dh (tinfo->deck);

    int hand_seq[ACE_HIGH];
    short int hand_suits[NUM_OF_SUITS];
    init (hand_seq, final_hand, hand_suits);

    if (MORE_OUTPUT)
    {
      do
      {
        printf ("%5s of %2s",
                get_card_face (tinfo->deck->card[i]), get_card_suit (tinfo->deck->card[i]));

        /* print newline every 4 cards */
        if (++j != 4)
          printf (" | ");
        else
        {
          CR;
          j = 0;
        }
      }
      while (++i < 52);
      CR;
    }

    if (SHOW_HAND)
      CR;

    /* +4 for the unimplemented PLAY feature */
    st_hand hand;

    /* Deal out a hand */

    do
    {
      if (PLAY && k < 5)
      {
        printf ("(%d)%5s of %2s", k + 1,
                get_card_face (tinfo->deck->card[i]), get_card_suit (tinfo->deck->card[i]));
        if (++j != 4)
          printf (" | ");
        else
        {
          CR;
          j = 0;
        }
      }

      hand.card[k].suit = tinfo->deck->card[i].suit;
      hand.card[k].face_val = tinfo->deck->card[i].face_val;

      /* Deal out every other card */
      i += 2;
    } while (++k < HAND + (PLAY * 4));

    int suitn, valuen;

    if (PLAY)
    {
      short discard = 0;
      int next = 5;
      for (j = 5; j < 9; j++)
      {
        scanf ("%hd", &discard);
        if (discard != 0)
        {
          suitn = hand.card[next].suit;
          hand.card[discard - 1].face_val = hand.card[next++].face_val;
          hand.card[discard - 1].suit = suitn;
        }
      }
      CR;
    }

    i = j = k = 0;
    do
    {
      if (SHOW_HAND)
      {
        printf ("%5s of %2s",
                get_card_face (tinfo->deck->card[i]), get_card_suit (tinfo->deck->card[i]));
        if (++j != 4)
          printf (" | ");
        else
        {
          CR;
          j = 0;
        }
      }

      /* Deal out every other card, out of the first 10 */
      i++;

    }
    while (++k < HAND);

    for (i = 0; i < HAND; i++)
    {

      /* if hand.card[i].face_value is 13 (King), valuen will equal 12
       * so hand_seq[12] will be incremented. If there are 3
       * Kings, hand_seq[12] will equal 3     */

      valuen = hand.card[i].face_val - 1;
      hand_seq[valuen]++;

      /* if hand.card[i].suit == 2 (Spades), then hand_suits[2] will
       * be incremented. If hand_suits[2] == 5, a flush will be
       * found when isFlush() is called.    */

      int suitn = hand.card[i].suit;
      hand_suits[suitn]++;
    }

    /* Evaluate the hand */
    short paired = find_matches (hand_seq, final_hand);

    /* if no matches were found, check for flush and straight
     * if there were any matches, flush or straight is impossible,
     * so don't bother checking               */

    bool isHighStraight = 0;

    if (!paired)
    {
      isStraight (hand_seq, &isHighStraight, final_hand);
      isFlush (final_hand, hand_suits);
    }

    hand_eval (tinfo->totals, ranks, isHighStraight, final_hand);
  }

  return NULL;
}


int
main (int argc, char *argv[])
{
  /* Number of hands to deal out */
  /* can be changed from the command line with -n [hands] */
  int RUN_COUNT = 20;
  MORE_OUTPUT = 0;
  SHOW_HAND = 0;
  PLAY = 0;

  getopts (argc, argv, &RUN_COUNT);

  if (RUN_COUNT > INT_MAX - 1)
  {
    printf ("Your RUN_COUNT should be lower than INT_MAX\n");
    return 1;
  }

  if (PLAY && MORE_OUTPUT)
  {
    MORE_OUTPUT = 0;
  }

  if (PLAY && SHOW_HAND)
  {
    SHOW_HAND = 1;
  }

  int totals[9];

  int i;
  for (i = 0; i < RANKS; i++)
      totals[i] = 0;

  st_deck_dh deck[num_threads];

  for (i = 0; i < num_threads; i++)
    deck_init_dh (&deck[i]);

  int s;
  struct thread_info *tinfo;
  pthread_attr_t attr;

  // How to calculate the needed stack size?
  int stack_size = 8192;

  void *res;

  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, stack_size);
  tinfo = calloc(num_threads, sizeof(struct thread_info));
  if (tinfo == NULL)
    puts("Unable to allocate memory");

  /* seeding the random number generator, used by deck_shuffle_dh() */
  srand (time (NULL));

  for (i = 0; i < num_threads; i++)
  {
    tinfo[i].deck = &deck[i];
    tinfo[i].RUN_COUNT = RUN_COUNT / num_threads;
    tinfo[i].totals = totals;
    s = pthread_create (&tinfo[i].thread_id, &attr, &main_thread, &tinfo[i]);
    if (!s)
      printf("\n Job %d started\n", i);
    else
      puts("Error creating thread");
  }

  s = pthread_attr_destroy(&attr);
  if (s != 0)
    puts("Error destroying attribute");

  for (i = 0; i < num_threads; i++)
  {
    s = pthread_join(tinfo[i].thread_id, &res);
    if (s != 0)
      printf("Error joining thread %d\n", i);
    else
      printf("Joined with thread %d; returned value was %s\n",
                       i, (char *) res);
    free(res);
  }

  free (tinfo);

  /* Show totals for all hands */
  show_totals (totals, ranks, RUN_COUNT);

  /* print a newline before the program ends */
  CR;
  return 0;
}
