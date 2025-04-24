[![C Build](https://github.com/theimpossibleastronaut/aapokerhands/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/theimpossibleastronaut/aapokerhands/actions/workflows/c-cpp.yml)

# aapokerhands
Will analyze for poker hands

I wrote this
(see [first uploaded version](https://github.com/andy5995/aapokerhands/commit/809629820fe14868cae5f2a675d8f51f55cd729c))
several years ago for practice. I've refactored some code since then.

## Dependencies

* [deckhandler](https://github.com/theimpossibleastronaut/deckhandler)
* [meson](http://mesonbuild.com/Quick-guide.html) (for the build system)

From the *aapokerhands* directory, to download latest *deckhandler* code:

    git submodule update --init --recursive

## Building

    meson setup builddir
    cd builddir
    ninja

## Usage
    ./aapokerhands: -n [hands] -s -v

# Downloads
[Releases](https://github.com/theimpossibleastronaut/aapokerhands/releases)

## Example Output

```
$ ./aapokerhands -n 100000
Out of 100000 hands:

                Pair:     42155
            Two Pair:      4717
     Three-of-a-Kind:      2144
            Straight:       411
               Flush:       188
          Full House:       150
      Four-of-a-Kind:        24
      Straight Flush:         0
         Royal Flush:         0


$ ./aapokerhands -n 1000000
Out of 1000000 hands:

                Pair:    421917
            Two Pair:     47511
     Three-of-a-Kind:     21150
            Straight:      3907
               Flush:      2020
          Full House:      1481
      Four-of-a-Kind:       224
      Straight Flush:        13
         Royal Flush:         5
```

## Installation

To specify the installation directory (defaults to /usr/local) and
install the program:

    From *builddir*: `meson configure --prefix=
    ninja install

To uninstall:

    ninja uninstall
