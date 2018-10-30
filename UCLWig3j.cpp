
#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "UCLWig3j.hpp"

int main(int argc, char** argv) {
	
	int ret_code = 0;
	
	try {
		
		if (argc == 4) {
			
			// Here we assume that the three arguments are j1, j2 and m.
			// Output is a list of (j, wig3j(j1, j2, j, m, -m, 0)) for all relevant values of j. Timing information is also provided.
			
			unsigned int j1 = atoi(argv[1]);
			unsigned int j2 = atoi(argv[2]);
			unsigned int m = atoi(argv[3]);
			
			unsigned int j12_max = ((j1 < j2) ? j2 : j1);
			unsigned int j12_min = ((j1 < j2) ? j1 : j2);
			
			std::vector<double> val_array(2 * j12_min + 1);
			auto start = std::chrono::high_resolution_clock::now();
			uclwig3j::uclwig3j(j1, j2, m, val_array.begin());
			auto  stop = std::chrono::high_resolution_clock::now();
			unsigned int j = (j1 < j2) ? j2 - j1 : j1 - j2;
			for (std::vector<double>::const_iterator cit = val_array.begin(); cit != val_array.end(); ++cit, ++j) {
				std::cout << j << "\t" << std::scientific << std::setprecision(16) << *cit << std::endl;
			}
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			std::cout << "Time taken = " << duration.count() << " microseconds" << std::endl;
			

		}
		else if (argc == 5) {
			
			// Here we assume that the four arguments are j1, j2, j and m.
			// We output the single value wig3j(j1, j2, j, m, -m, 0).
			
			unsigned int j1 = atoi(argv[1]);
			unsigned int j2 = atoi(argv[2]);
			unsigned int j = atoi(argv[3]);
			unsigned int m = atoi(argv[4]);
			
			unsigned int j12_max = ((j1 < j2) ? j2 : j1);
			unsigned int j12_min = ((j1 < j2) ? j1 : j2);
			
			if (((j < (j12_max - j12_min))) || (j > (j12_max + j12_min))) {
				std::cout << 0.0 << std::endl;
			}
			else {
				std::vector<double> val_array(2 * j12_min + 1);
				uclwig3j::uclwig3j(j1, j2, m, val_array.begin());
				double wig3j = val_array[j - (j12_max - j12_min)]; // A bit wasteful as we only pick out one value from the whole array returned!
				std::cout << std::scientific << std::setprecision(16) << wig3j << std::endl;
			}
		}
		else {
			throw "Must provide either three arguments (j1, j2, m) or four arguments (j1, j2, j, m)";
		}
	}
	catch (const char* emsg) {
		std::cerr << "Fatal error: " << emsg << std::endl;
		ret_code = 1;
	}
	return ret_code;


}

