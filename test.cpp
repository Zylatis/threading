#include "thread_classes.h"

int main ( int argc, char *argv[] ){
	
	
	Semaphore sem(2);

	thread t1(do_stuff,"X",1,ref(sem) ) ; // need ref here
	thread t2(do_stuff,"Y",2, ref(sem) );
	thread t3(do_stuff,"Z",3,ref(sem) ) ; // need ref here
	thread t4(do_stuff,"A",4, ref(sem) );
	

	t1.join();
	t2.join();
	t3.join();
	t4.join();
		
	return 0;
}
