all:
	g++  -O3 -std=c++11 -o threading_tests threading_tests.cpp -pthread
clean:
	$(RM) threading_tests
