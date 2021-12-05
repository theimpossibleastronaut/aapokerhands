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

    meson builddir
    cd builddir
    ninja

The resulting binary will be in *src/*

## Usage
    src/aapokerhands: -n [hands] -s -v

# Downloads
[Releases](https://github.com/theimpossibleastronaut/aapokerhands/releases)

## Example Output

```
andy@oceanus:~/src/aapokerhands$ ./aapokerhands -n 100000
Out of 100000 hands:

                Pair:     42250
            Two Pair:      4683
     Three-of-a-Kind:      2081
            Straight:       422
               Flush:       200
          Full House:       137
      Four-of-a-Kind:        18
      Straight Flush:         3
         Royal Flush:         0

andy@oceanus:~/src/aapokerhands/src$ ./aapokerhands -n 1000000
Out of 1000000 hands:

                Pair:    422821
            Two Pair:     47282
     Three-of-a-Kind:     20919
            Straight:      3996
               Flush:      1973
          Full House:      1533
      Four-of-a-Kind:       237
      Straight Flush:        15
         Royal Flush:         1

andy@oceanus:~/src/aapokerhands/src$ nice -n 19 ./aapokerhands -n 1000000000
Out of 1000000000 hands:

                Pair: 422573272
            Two Pair:  47540875
     Three-of-a-Kind:  21118955
            Straight:   3927170
               Flush:   1963944
          Full House:   1439822
      Four-of-a-Kind:    241218
      Straight Flush:     13920
         Royal Flush:      1552
```

[More Example Output](https://github.com/theimpossibleastronaut/aapokerhands/blob/master/example_output01.txt)

## Installation

To specify the installation directory (defaults to /usr/local) and
install the program:

    From *builddir*: `meson configure --prefix=
    ninja install

To uninstall:

    ninja uninstall
