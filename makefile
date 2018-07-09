
uclwig3j : uclwig3j.o 
	${CXX} -lm -o uclwig3j uclwig3j.o

uclwig3j.o: uclwig3j.cpp uclwig3j.hpp
	${CXX} -std=c++11 -c -o uclwig3j.o uclwig3j.cpp
 
clean: 
	rm -f uclwig3j
	rm -f uclwig3j.o 
