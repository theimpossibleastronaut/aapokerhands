[![Build Status](https://travis-ci.org/theimpossibleastronaut/aa-pokerhands.svg?branch=master)](https://travis-ci.org/theimpossibleastronaut/aa-pokerhands)

# aa-pokerhands
Will analyze for poker hands

I wrote this
(see [first uploaded version](https://github.com/andy5995/aa-pokerhands/commit/809629820fe14868cae5f2a675d8f51f55cd729c))
6 years ago.

## Building
    ./configure
    make

The binary will be built in src/

## Usage
    ./pokerhands: -n [hands] -s -v

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

```

[More Example Output](https://github.com/theimpossibleastronaut/aa-pokerhands/blob/master/example_output01.txt)

## October 23, 2017
I may work on this occasionally, or I may not. If you'd like to add or
improve the code, will review PRs. I hope someone finds this program
educational (or fun).
