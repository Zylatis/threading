all:
	g++  -O3 -std=c++11 -o threading_tests threading_tests.cpp -fopenmp

clean:
	$(RM) threading_tests
