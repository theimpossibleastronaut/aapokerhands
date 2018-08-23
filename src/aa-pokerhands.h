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

#include "config.h"

/* Number of hands to deal out */
/* can be changed from the command line with -n [hands] */
int RUN_COUNT;

/* can be set on the command line with -v */
/* Show all 52 cards after the deck has been shuffled */
bool MORE_OUTPUT;

/* can be set on the command line with -s */
/* Show the hand that was dealt */
bool SHOW_HAND;

/* Leave set to 0, PLAY feature not finished */
bool PLAY;

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

char *faces[13];

bool final[RANKS];

bool high_straight;

typedef short int loop;
