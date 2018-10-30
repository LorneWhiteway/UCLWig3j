
UCLWig3j : UCLWig3j.o 
	${CXX} -lm -o UCLWig3j UCLWig3j.o

UCLWig3j.o: UCLWig3j.cpp UCLWig3j.hpp
	${CXX} -std=c++11 -c -o UCLWig3j.o UCLWig3j.cpp
 
clean: 
	rm -f UCLWig3j
	rm -f UCLWig3j.o 
