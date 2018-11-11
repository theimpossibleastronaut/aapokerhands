AUTOMAKE_OPTIONS = subdir-objects foreign no-dependencies

# Build a static library. This really isn't needed for a program this
# small; normally we'd just add deckhandler.c to the _SOURCES field, but
# as this is a practice project, we're going to make a static library
#
# the "noinst" directive means that the libary will not be installed
noinst_LIBRARIES = libdeckhandler.a
libdeckhandler_a_SOURCES = deckhandler/deckhandler.c

# Now define the binary
bin_PROGRAMS = pokerhands
AM_CFLAGS = -std=c99 -Wall
pokerhands_SOURCES =	aa-pokerhands.c \
			functions.c

# Add the library
pokerhands_LDADD = libdeckhandler.a

# this tells 'make dist' to add the required header files, otherwise they
# won't be included in a distribution archive
noinst_HEADERS = \
	aa-pokerhands.h	\
	functions.h	\
	deckhandler/deckhandler.h
