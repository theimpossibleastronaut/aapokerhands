/*
 * functions.c
 *
 * Copyright 2011-2020 Andy Alt <andy400-dev@yahoo.com>
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

#include <stdlib.h>
#include <pthread.h>
#include "aa-pokerhands.h"
#include "functions.h"

void
init (int *hand_seq, bool *final_hand, short int* hand_suits)
{
  int i;

  for (i = 0; i < ACE_HIGH; i++)
    hand_seq[i] = 0;

  for (i = 0; i < NUM_OF_SUITS; i++)
    hand_suits[i] = 0;

  /* set all array elements to 0 */
  for (i = 0; i < RANKS; i++)
    final_hand[i] = false;
}

void
isStraight (int *hand_seq, bool *isHighStraight, bool *final_hand)
{
  int k = 13;

  short state = 0;

  /* Copy ACES for high straight/royal flush checking */
  if (hand_seq[0] == 1)
    hand_seq[13] = 1;

  /* The hand is never sorted numerically, instead, just look for
   * 5 consecutive 1's.*/
  do
  {

    /* May be a baby straight, not breaking if k == 13 */
    if (hand_seq[k] == 1 && hand_seq[k - 1] == 0 && k != 13)
    {
      break;
    }
    else if (hand_seq[k] == 1 && hand_seq[k - 1] == 1)
    {
      state++;
      if (state == 4)
      {
        final_hand[STRAIGHT] = 1;
        if (k == 10)
        {
          /* printf("High Straight"); */
          *isHighStraight = 1;
        }
      }
    }

  }
  while (--k && !final_hand[STRAIGHT]);
}

bool
is_flush (short int* hand_suits)
{
  int i;
  for (i = 0; i < NUM_OF_SUITS; ++i)
  {
    if (hand_suits[i] != HAND && hand_suits[i])
      break;
    else if (hand_suits[i] == HAND)
      return true;
  }
  return false;
}

static void
usage (const char *argv_one)
{
  printf ("Usage: %s: -n [hands] -s -v -V\n", argv_one);
  printf ("\n\
-n [hands]          number of hands to deal\n\
-s                  show the hands that were dealt\n\
-v                  show the deck for each hand\n\
-V                  display Version information\n\
");

  exit (1);
}

void
getopts (int argc, char *argv[], int *RUN_COUNT)
{
  /* fetch command line arguments */
  int i;
  if (argc > 1)
  {
    for (i = 1; i < argc; i++)
      if (argv[i][0] == '-')
        switch (argv[i][1])
        {
        case 'n':
          if (i < argc - 1)
          {
            *RUN_COUNT = atoi (argv[i++ + 1]);
            if (*RUN_COUNT < 1)
              usage (argv[0]);
          }
          else
            usage (argv[0]);
          break;
        case 'v':
          MORE_OUTPUT = 1;
          break;
        case 's':
          SHOW_HAND = 1;
          break;
        case 'V':
          printf ("%s\n", PACKAGE_STRING);
          break;
        default:
          usage (argv[0]);
          break;
        }
  }

  else
    usage (argv[0]);
}

short int
find_matches (int *hand_seq, bool *final_hand)
{
  int face_val;
  short paired = 0;

  for (face_val = 0; face_val < ACE_HIGH; ++face_val)
  {
    if (hand_seq[face_val] > 1)
    {
      paired++;
    }
    if (hand_seq[face_val] == 2 && !final_hand[PAIR])
    {
      final_hand[PAIR] = 1;
    }
    else if (hand_seq[face_val] == 2)
    {
      final_hand[TWO_PAIR] = 1;
      final_hand[PAIR] = 0;
    }
    else if (hand_seq[face_val] == 3)
      final_hand[THREE_OF_A_KIND] = 1;
    else if (hand_seq[face_val] == 4)
    {
      final_hand[FOUR_OF_A_KIND] = 1;
      break;
    }
    if (paired > 1)
      break;
  }
  return paired;
}

void
show_totals (int *totals, const char **ranks, int RUN_COUNT)
{
  int n;

  printf ("Out of %d hands:\n\n", RUN_COUNT);

  for (n = 0; n < RANKS; n++)
  {
    printf ("%20s: %9d\n", ranks[n], totals[n]);
  }
}

void
hand_eval (int *totals, const char **ranks, bool isHighStraight, bool *final_hand)
{

  short eval = -1;

  if (final_hand[PAIR] && final_hand[THREE_OF_A_KIND] != 1)
  {
    eval = PAIR;
    totals[PAIR]++;
  }
  else if (final_hand[TWO_PAIR])
  {
    eval = TWO_PAIR;
    totals[TWO_PAIR]++;
  }
  else if (final_hand[THREE_OF_A_KIND] && final_hand[PAIR] != 1)
  {
    eval = THREE_OF_A_KIND;
    totals[THREE_OF_A_KIND]++;
  }
  else if (final_hand[STRAIGHT] && final_hand[FLUSH] != 1)
  {
    eval = STRAIGHT;
    totals[STRAIGHT]++;
  }
  else if (final_hand[FLUSH] == 1 && final_hand[STRAIGHT] != 1)
  {
    eval = FLUSH;
    totals[FLUSH]++;
  }
  else if (final_hand[PAIR] && final_hand[THREE_OF_A_KIND])
  {
    /* final_hand[FULL_HOUSE] = 1; */
    eval = FULL_HOUSE;
    totals[FULL_HOUSE]++;
  }
  else if (final_hand[FOUR_OF_A_KIND])
  {
    eval = FOUR_OF_A_KIND;
    totals[FOUR_OF_A_KIND]++;
  }
  else if (final_hand[STRAIGHT] && final_hand[FLUSH] && isHighStraight != 1)
  {
    /* final_hand[7] = 1;    */
    eval = STRAIGHT_FLUSH;
    totals[STRAIGHT_FLUSH]++;
  }
  else if (final_hand[STRAIGHT] && final_hand[FLUSH] && isHighStraight)
  {
    eval = ROYAL_FLUSH;
    totals[ROYAL_FLUSH]++;
  }

  if (SHOW_HAND)
  {
    printf ("\n\t\t-->");
    if (eval > -1)
      printf ("%s", ranks[eval]);
    else
      printf ("Nothing");

    printf ("<--\n\n");
  }

  return;
}
