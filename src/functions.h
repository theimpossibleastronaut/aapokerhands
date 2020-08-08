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

void init (int *hand_seq, bool *final, short int* hand_suits);

void isStraight (int *hand_seq, bool *isHighStraight, bool *final);

bool is_flush (short int* hand_suits);

void getopts (int argc, char *argv[], int *RUN_COUNT);

short int find_matches (int *hand_seq, bool *final);

void hand_eval (int *totals, const char **ranks, bool isHighStraight, bool *final);

void
show_totals (int *totals, const char **ranks, int RUN_COUNT);
