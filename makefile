all:
	g++  -O3 -std=c++11 -o test test.cpp -fopenmp

clean:
	$(RM) test
