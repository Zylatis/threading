using namespace std;
//~ #include "catch.hpp"
// TESTBED FILE FOR THREAD_CLASSES.H
// Below is just a basic test run of some tasks which make use of the Semaphore and Task classes
#include "thread_classes.h"


int main ( int argc, char *argv[] ){

	// A semaphore is a way to control the number of threads accessing a resource
	// for example if you only want to allow 2 threads to write to disk at a given time
	// you could declare "Semaphore disk(2,0.1);" and any time more than 2 threads try to lock this
	// the remaining threads will have to wait (though not necessarily in order!) Once one thread releases the lock
	// another can join in, but no more than 2 can have lock. See class definition for more details.
	//~ Semaphore sem(1,0.1);
	
	// A simple test can be done with 4 threads without using the Task() class
	// We pass the semaphore by reference and so require the ref() wrapper 'because threads library' apparently
	// The worker() function simply tries to get the resource protected by 'sem' and then prints a char and its ID,
	// and then releases the lock.
	// Does some really crappy and basic print to screen stuff to indicate which threads are waiting
	// for resources to free up.
	//~ thread t1( worker, "X", 1, ref(sem) ) ;
	//~ thread t2( worker, "Y", 2, ref(sem) );
	//~ thread t3( worker, "Z", 3, ref(sem) ) ; 
	//~ thread t4( worker, "A", 4, ref(sem) );

	
	//~ t1.join();
	//~ t2.join();
	//~ t3.join();
	//~ t4.join();
	

	// setup a task with 1 semaphore
	//~ TaskA taskA( { &sem });
	//~ thread t1( &TaskA::work, taskA, 1 ) ;
	//~ thread t2( &TaskA::work, taskA, 2 ) ;
	
	// will segfault if join not done, should do some kind of testing/metaprogramming here...
	//~ t1.join();
	//~ t2.join();
	
	
	Semaphore s1(2,0.1), s2(1,0.1);
	
	// In this case we can have A and B run, then A stop and C start befeore A finishes
	// because we have 2 units of resource 1 (s1) to use
	TaskA A( { &s1 }), B({  &s1, &s2} ), C({&s1});
	thread t1( &TaskA::work, A, 1 ) ;
	thread t2( &TaskA::work, B, 2 ) ;
	thread t3( &TaskA::work, C, 3 ) ;	

	t1.join();
	t2.join();
	t3.join();



	return 0;
}
