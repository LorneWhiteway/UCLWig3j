# uclwig3j

## Introduction

This project includes a C++ header file, uclwig3j.hpp, that implements a fast calculation of sets of Wigner 3j symbols for certain (simple) inputs.

## Wigner 3j symbols

See http://mathworld.wolfram.com/Wigner3j-Symbol.html for a definition of the Wigner 3j symbol. In what follows we write the symbol as wig3j(a, b, c, alpha, beta, gamma) instead of a more common notation that spans two lines.

The expected use case for this code is as described in Appendix A of https://arxiv.org/pdf/astro-ph/0410394.pdf, equations A12 - A17. These equations require the calculation of wig3j(j1, j2, j, m, -m, 0) where j1 and j2 are given non-negative integers, m is 0 or 2, and the integer j runs over all relevant values (i.e. |j1-j2| to j1+j2 inclusive). We assume that j1 and j2 may be as large as 10,000. The calculation of this set of wig3j values may be done efficiently using the recurrence relation described in Schulten and Gorden (Computer Physics Communications 11 (1976) 269-278; http://www-s.ks.uiuc.edu/Publications/Papers/PDF/SCHU76B/SCHU76B.pdf).

## Installation

Begin by cloning the repository at https://github.com/Astrophysics-UCL/uclwig3j.git; this will create a directory `uclwig3j`.

If you do not need to run the test cases, then there is nothing more to do. The directory contains a C++ header file `uclwig3j.hpp` that can be included in your code. This header file contains a namespace `uclwig3j` in which there is a function also called `uclwig3j`.

To use the function in your C++ code:
1. Include the header file `uclwig3j.hpp`.
1. Allocate space for `2*min(j1, j2) + 1` doubles. The allocated space may be an array of doubles, or a std::vector<double>, or any other suitable container. The set it_begin` to be an iterator (or pointer) pointing to the beginning of the allocated space.
1. Call `uclwig3j` with inputs j1, j2 and m (all unsigned ints) followed by it_begin. The function will fill the container with the values wig3j(j1, j2, j, m, -m, 0) for |j1-j2| <= j <= j1+j2.

Example 1 (using an array of doubles)

```
#include uclwig3j.hpp
unsigned int j1 = 300;
unsigned int j2 = 200;
unsigned int m = 2;

double* array = new double[2 * ((j1 < j2) ? j1 : j2) + 1]; // Remember that this will need to be freed at some time.
uclwig3j::uclwig3j(j1, j2, m, array);
```

Example 2 (using a std::vector)
```
#include uclwig3j.hpp
#include <vector>
unsigned int j1 = 300;
unsigned int j2 = 200;
unsigned int m = 2;

std::vector<double> array(2 * ((j1 < j2) ? j1 : j2) + 1);
uclwig3j::uclwig3j(j1, j2, m, array.begin());
```

## Testing

To run the test code:
1. Call `make`; this will build an executable `uclwig3j`. (If you want to call this executable directly yourself, then pass on the command line either `j1 j2 j m` (in which case the function will return wig3j(j1, j2, j, m, -m, 0)) or `j1 j2 m` (in which case the function will return a complete list of j and wig3j(j1, j2, j, m, -m, 0) values as well as information on how long the calculation took.)
2. Call the Python 2 module `uclwig3j_test.py` with the command line parameter `run`. This will read the provided file 'uclwig3j_test.txt' (which contains precomputed correct values for wig3j, calculated using wigxjpf (see http://fy.chalmers.se/subatom/wigxjpf/)) and then summarise the maximum absolute error and maximum percentage error between the correct values and the uclwig3j values.

## Test results

Currently the test file 'uclwig3j_test.txt' contains:
* test cases for all wig3j(j1, j2, j, m, -m, 0) in which j1 <= 25, j2 <= 25, j is in the relevant range, and m = 0 or 2. 
* 20,000 further randomly chosen test cases in which j1 <= 10,000, j2 <= 10,000, j is in the relevant range, and m = 0 or 2.

With these test cases, the maximum observed errors are:

Maximum absolute error:
j1 = 0; j2 = 7894; j = 7894; m = 0; correct wig3j = 7.95834363359204482e-03; our calculated wig3j = 7.95834363359159899e-03; absolute error = 4.45823933326e-16

Maximum percentage error:
j1 = 4329; j2 = 9656; j = 13079; m = 2; correct wig3j = -1.55805641780838282e-07; our calculated wig3j = -1.55805641781792783e-07; absolute error = 9.54500252437e-19; percentage error = 6.12621064988e-12

Maximum absolute error when correct value is zero:
j1 = 3; j2 = 3; j = 2; m = 2; correct wig3j = 0.00000000000000000e+00; our calculated wig3j = -5.55111512312578270e-17; absolute error = 5.55111512313e-17
