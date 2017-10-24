#
# Makefile for aa-pokerhands
#
#

fifteen: aa-pokerhands.c
	gcc -Wall -Werror aa-pokerhands.c -o pokerhands

clean:
	rm -f *.o a.out pokerhands
