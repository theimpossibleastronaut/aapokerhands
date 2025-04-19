/*
 lib.c
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

#include <stdio.h>

#include "lib.h"

const char *ranks[NUM_HAND_RANKS] = {[PAIR] = "Pair",
                                     [TWO_PAIR] = "Two Pair",
                                     [THREE_OF_A_KIND] = "Three-of-a-Kind",
                                     [STRAIGHT] = "Straight",
                                     [FLUSH] = "Flush",
                                     [FULL_HOUSE] = "Full House",
                                     [FOUR_OF_A_KIND] = "Four-of-a-Kind",
                                     [STRAIGHT_FLUSH] = "Straight Flush",
                                     [ROYAL_FLUSH] = "Royal Flush"};

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

static bool isStraight(int *hand_seq, bool *final_hand) {
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
          return true;
        }
      }
    }

  } while (--k && !final_hand[STRAIGHT]);

  return false;
}

static bool is_flush(const short int *hand_suits) {
  int i;
  for (i = 0; i < NUM_OF_SUITS; ++i) {
    if (hand_suits[i] != HAND_SIZE && hand_suits[i])
      break;
    else if (hand_suits[i] == HAND_SIZE)
      return true;
  }
  return false;
}

static short int find_matches(int *hand_seq, bool *final_hand) {
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

short hand_eval(int *hand_seq, const short int *hand_suits, bool *final_hand) {
  short eval = -1;

  short paired = find_matches(hand_seq, final_hand);
  bool isHighStraight = false;

  if (!paired) {
    isHighStraight = isStraight(hand_seq, final_hand);
    final_hand[FLUSH] = is_flush(hand_suits);
  }

  if (final_hand[STRAIGHT] && final_hand[FLUSH] && isHighStraight)
    return ROYAL_FLUSH;
  if (final_hand[STRAIGHT] && final_hand[FLUSH])
    return STRAIGHT_FLUSH;
  if (final_hand[FOUR_OF_A_KIND])
    return FOUR_OF_A_KIND;
  if (final_hand[PAIR] && final_hand[THREE_OF_A_KIND])
    return FULL_HOUSE;
  if (final_hand[FLUSH] && !final_hand[STRAIGHT])
    return FLUSH;
  if (final_hand[STRAIGHT] && !final_hand[FLUSH])
    return STRAIGHT;
  if (final_hand[THREE_OF_A_KIND] && final_hand[PAIR] != 1)
    return THREE_OF_A_KIND;
  if (final_hand[TWO_PAIR])
    return TWO_PAIR;
  if (final_hand[PAIR] && final_hand[THREE_OF_A_KIND] != 1)
    return PAIR;

  return eval;
}
