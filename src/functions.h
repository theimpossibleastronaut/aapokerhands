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

void shuffle(void);

void zero(void);

void make_real(void);

void isStraight(void);

void isFlush(void);

void getopts (int argc, char *argv[]);

short int find_matches(void);

void hand_eval(int run_count, const char **ranks);

void usage(const char *argv_one);
