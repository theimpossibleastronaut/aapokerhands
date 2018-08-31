[![Build Status](https://travis-ci.org/theimpossibleastronaut/aa-pokerhands.svg?branch=master)](https://travis-ci.org/theimpossibleastronaut/aa-pokerhands)

# aa-pokerhands
Will analyze for poker hands

I wrote this
(see [first uploaded version](https://github.com/andy5995/aa-pokerhands/commit/809629820fe14868cae5f2a675d8f51f55cd729c))
6 years ago.

## Dependencies

[deckhandler](https://github.com/theimpossibleastronaut/deckhandler)

From the *aa-pokerhands* directory, to download latest *deckhandler* code:

    git submodule update --init --recursive

## Building

    mkdir build
    cd build
    ../configure
    make

The binary will be built in src/

## Usage
    ./pokerhands: -n [hands] -s -v

# Downloads
[Releases](https://github.com/theimpossibleastronaut/aa-pokerhands/releases)

## Example Output

```
andy@oceanus:~/src/aa-pokerhands$ ./pokerhands -n 100000
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

andy@oceanus:~/src/aa-pokerhands/src$ ./pokerhands -n 1000000
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

andy@oceanus:~/src/aa-pokerhands/src$ nice -n 19 ./pokerhands -n 1000000000
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

[More Example Output](https://github.com/theimpossibleastronaut/aa-pokerhands/blob/master/example_output01.txt)

## October 23, 2017
I may work on this occasionally, or I may not. If you'd like to add or
improve the code, will review PRs. I hope someone finds this program
educational (or fun).
