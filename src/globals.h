/*
 * globals.h
 *
 * Copyright 2011-2023 Andy Alt <arch_stanton5995@proton.me>
 * This file is part of aapokerhands
 * <https://github.com/theimpossibleastronaut/aapokerhands>
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

#include "config.h"

/* use deckhandler library */
#include "deckhandler/deckhandler.h"

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

const int n_ranks = sizeof(ranks) / sizeof(ranks[0]);

/* can be set on the command line with -v */
/* Show all 52 cards after the deck has been shuffled */
extern bool MORE_OUTPUT;

/* can be set on the command line with -s */
/* Show the hand that was dealt */
extern bool SHOW_HAND;

extern bool PLAY;

enum
{ PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT, FLUSH,
  FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH
};

#define ACE_HIGH 14

#define HAND 5

#define CR printf("\n")

typedef struct
{
  st_card_info_dh card[HAND + 4];
} st_hand;
