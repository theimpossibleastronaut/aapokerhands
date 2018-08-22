/*
 * aa-pokerhands.h
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

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

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
