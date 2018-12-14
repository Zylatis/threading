#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
using namespace std;

#include "headers/catch.hpp"
#include "../imports.h"
#include "../thread_classes.h"


TEST_CASE( "Semaphore timeout", "[semaphore]" ) {
	// check that having a zero resource semaphore times out
	Semaphore test(0,1);
    REQUIRE(test.lock() == false);
}
	
