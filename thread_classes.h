#include "imports.h"
#include <thread>
#include <mutex>
#include <condition_variable>


// binary semaphore is just a signalling mechanism
// my guess: represents a single flag that can be toggled
// which then notifies other threads of toggle (or should other threads periodically check?)
// basically a mutex? come back to this...

class Semaphore {
	private:
		condition_variable cv;
		mutex m;

	public:
		int count;
		Semaphore( int x ){ count = x; };
	
	void lock(){
		unique_lock<mutex> lk(m);
		if(count == 0){
			cv.wait(lk);
		}
		
		count--; 
	}
	
	void unlock( ){
		cv.notify_one();
		unique_lock<mutex> lk(m);
		count++;
	}
};


void do_stuff( string msg, int id, Semaphore &sem){
	cout<<id<<" trying to lock, count = "<<sem.count<<endl;
	sem.lock();
	cout<<id<<" locked"<<endl;
	int x = 0;
	for(int i = 0; i<pow(10,9);i++){
		x++;
	}
	cout<<x<<endl;	
	sem.unlock();
	cout<<id<<" unlocked"<<endl;
}

