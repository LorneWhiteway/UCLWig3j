#ifndef UCLWIG3J_HPP
#define UCLWIG3J_HPP

#include <cmath>

namespace uclwig3j {

	// Calculates wig3j(j1, j2, j, m, -m, 0) in this starting case in which j=abs(j1-j2).
	// See my notes p. E20 for the m = 0 case.
	double initial_value(unsigned int j12_max, unsigned int j12_min, unsigned int m) {
		
		unsigned int j12_diff = j12_max - j12_min;
		
		// Special case. See my notes p. 68
		if (m == 2 && j12_min < 2) {
			return 0.0;
		}
		else {
			double sym_3j = 1.0;
			double factor1 = 1.0;
			double factor2 = 1.0;
			double factor3a = ((m == 0) ? j12_min + 1.0 : j12_min - 1.0);
			double factor3b = ((m == 0) ? j12_min + 1.0 : j12_min + 3.0);
			double factor4 = 2 * j12_min + 1.0;
			
			
			for (unsigned int i = 0; i < j12_diff; ++i) {
				sym_3j *= ((factor1++ / factor2) * (factor3a / factor4++));
				sym_3j *= ((factor1++ / factor2) * (factor3b / factor4++));
				factor2++;
				factor3a++;
				factor3b++;
			}
			
			sym_3j /= (2 * j12_max + 1.0);
			sym_3j = std::sqrt(sym_3j);
			
			// Set the sign.
			sym_3j *= ((j12_max % 2 == 0) ? 1.0 : -1.0);
			
			return sym_3j;
		}
		
	}

	double second_value(unsigned int j12_max, unsigned int j12_min, unsigned int m, double initial_value) {
		
		unsigned int j12_diff = j12_max - j12_min;
		
		if (m == 0) {
			return 0.0;
		}
		else {
			if (j12_diff == 0) {
				// Calculate wig3j(j1, j2, 1, 2, -2, 0) - The general formula doesn't work here. See p. 60 for explanation, and p. 63 for the required formula.
				if (j12_max == 1) {
					return 0.0;
				}
				else {
					// By chance this formula works for j12_max = 2 as well as for j12_max > 2 (different derivations of the formula required in these two cases. See p. 65.)
					return ((j12_max % 2 == 0) ? 1.0 : -1.0) * 2.0 / std::sqrt(j12_max * (j12_max + 1.0) * (2.0 * j12_max + 1.0));
				}
			}
			else {
				// See p. 81.
				return initial_value * 2.0 * std::sqrt((2.0 * j12_diff + 1.0) / (j12_min * (j12_max + 1.0)));
			}
		}
	}


	// it_begin should point to the start of a container of doubles of the correct size (which is 2 * min(j1, j2) + 1).
	template <class Iter> void uclwig3j(unsigned int j1, unsigned int j2, unsigned int m, Iter it_begin) {
		
		if (m != 0 && m != 2) {
			throw "m must be zero or two";
		}

		unsigned int j12_min = ((j1 < j2) ? j1 : j2);
		unsigned int j12_max = ((j1 < j2) ? j2 : j1);
		unsigned int j12_sum = j12_max + j12_min;
		unsigned int j12_diff = j12_max - j12_min;
		
		Iter it = it_begin;
		
		if (m == 0) {
			
			unsigned int j = j12_diff;
			double sym_3j = initial_value(j12_max, j12_min, m);
			
			*(it++) = sym_3j;
			j += 2;
			unsigned int f1 = 1;
			unsigned int f2 = 2 * j12_diff + 1;
			unsigned int f3 = 2 * j12_max + 2;
			unsigned int f4 = 2 * j12_min - 1;
			
			for (; j <= j12_sum; j += 2, f1 += 2, f2 += 2, f3 += 2, f4 -= 2 /*'it' is incremented in the loop body*/) {
				
				*(it++) = 0.0; // This refers to j-1...
				
				double factor = (f1 / (f1 + 1.0)) * (f2 / (f2 + 1.0)) * (f3 / (f3 + 1.0)) * ((f4 + 1.0) / f4);
				factor = -std::sqrt(factor);
				sym_3j *= factor;
				*(it++) = sym_3j;
				
			}
		}
		else {

			// First value:
			double starting_value = initial_value(j12_max, j12_min, m);
			*(it++) = starting_value;
			// Second value:
			if (j12_min > 0) {
				*(it++) = second_value(j12_max, j12_min, m, starting_value);
			}
			// Third and remaining values:
			for (unsigned int j = j12_diff + 2; j <= j12_sum; ++j /*'it' is incremented in the loop body*/) {
				
				// See p. 82
				double factor = 1.0 / std::sqrt(double(j - j12_diff) * (j + j12_diff) * (j12_sum + 1 - j) * (j12_sum + 1 + j));
				double coeff1 = 4.0 * (2.0 * j - 1.0) * factor;
				double coeff2 = -1.0 * std::sqrt(double(j - 1.0 - j12_diff) * (j - 1.0 + j12_diff) * (j12_sum + j) * (j12_sum + 2.0 - j)) * factor;
				
				// The next line implements the recurrence formula.
				double sym_3j = coeff1 * *(it - 1) + coeff2 * *(it - 2);
				*(it++) = sym_3j;
			
			}
		}
	}
}

#endif //UCLWIG3J_HPP
