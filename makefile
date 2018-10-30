
run_UCLWig3j : run_UCLWig3j.o 
	${CXX} -lm -o run_UCLWig3j run_UCLWig3j.o

run_UCLWig3j.o: run_UCLWig3j.cpp UCLWig3j.hpp
	${CXX} -std=c++11 -c -o run_UCLWig3j.o run_UCLWig3j.cpp
 
clean: 
	rm -f run_UCLWig3j
	rm -f run_UCLWig3j.o 
