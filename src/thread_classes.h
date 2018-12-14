#include "imports.h"

auto sec = chrono::milliseconds(1); // pinch from fancy chrono namespace for use in timeout in semaphore

// Semaphore class to ensure only a set number of threads can
// use a given resource at any time (can also be 1 which is binary semaphore and, sort of, a mutex)
class Semaphore {
	private:
		// Used to signal waiting threads
		condition_variable cv; 
		
		// Mutex used to ensure the semaphore status itself does not get race condition shit
		mutex m_; 
		
		// Max number of threads which can lock this
		int n_;
		
		// Time until waiting thread gives up and returns timeout flag
		// Default timeout is infinity (see constructor below), i.e. wait as long as needed
		double timeout_; 
	public:
		
		
		Semaphore( int x, double y = 1.*INT_MAX ) : n_(x), timeout_(y) { 
		
		};
	
	// Method for thread to attempt to lock the semaphore and acquire the resource
	bool lock(){
		// Define a unique lock for this semaphores mutex. unique lock just means it unlocks when going out of scope
		unique_lock<mutex> lk(m_);
		
		// A condition variable status object is used to capture the output of the wait_for() call
		cv_status status;
		
		// If no resources are available, try to wait it out
		if(n_ == 0){
			// Tell the thread attempting the lock to wait for a signal from another thread saying
			// it has released some resources
			status = cv.wait_for(lk, timeout_*sec); 
			
			// If the thread reaches timeout for this lock, tell thread it didn't work
			// and it's up to the thread/task to know what to do with that info
			if( status == cv_status::timeout ){
				return false;
			}
		}		
		// If lock is successful, reduce the available counter by one, return successful
		n_--; 
		return true;
	}
	
	// Method to unlock this mutex and notify another thread (as determind by OS scheduler)
	void unlock( ){
		// Lock semaphore to prevent race condition fuckery
		unique_lock<mutex> lk(m_);		
		// Add back resource released by this thread
		n_++;
		// Notify another thread it can begin work
		cv.notify_one();
			
		// Lock released exiting scope
	}
};

// Attempt at an encapsulated Task() parent class
// The idea here is to enable tasks that can vary in their resource requirements
// and still try to avoid deadlocks dynamically (in a sense having a monitor)
// Requirements must be determined at instantiation
class Task {
	protected:
		// Number of requirements
		int nreq;
		
		// List of resources, semaphores, that the task needs
		// Ideally, in same order as all other tasks requiring the same resources which can avoid
		// simple deadlocks just by construction (i.e. two tasks both needing A and B)
		vector<Semaphore*> req_resources;
		
		// A stack to keep track of locks this task has acquired, so if it cannot
		// get all of them it can easily go back and unlock those it already has and start again
		stack<Semaphore*> locked;
		
	public:
		// Constructor (explicit because of derived classes, see below - also read more)
		explicit Task( vector<Semaphore*> reqs ){
			req_resources = reqs;
			nreq = reqs.size();
			
		}
		
		// Method to try and acquire all the resources needed to continue
		// However, if at any point any one of its locking attempts times out,
		// it unlocks everything it got up to that point and starts again.
		// Possibly better ways but not sure yet. Going up the stack and trying again might work,
		// but mutex calls are fucking expensive so this tries to minimize that
		void get_resources(){	
			
			// Assume for now we cannot continue (no locks)
			bool can_proceed = false;
			// Start by looking for first lock
			int resource = 0;
			// While we don't have all the locks, try to acquire next lock
			while( resource != nreq ){
				// Tries to lock semaphore, which as above returns 0 (fail) or 1 (succ)
				can_proceed = req_resources[resource]->lock();
				
				
				if(!can_proceed){
					// If semaphore lock fails, release all locks and start from first lock again
					release_all();
					resource = 0;
				} else {
					// Otherwise, add the lock to the stack and continue to next one
					locked.push( req_resources[resource] );
					resource++;
				}
			}
			// should probably add timeout checks here,
		}
		
		// Goes through the stack holding the locks and releases them all.
		void release_all( ){
			Semaphore* res;
			while(!locked.empty()){
				res = locked.top();
				locked.pop();
				res->unlock();
			}
		}
		
		void test_work(){
			// Put unit tests here (size of stack etc)
			get_resources();
			release_all();
		}
		
		
		// Worker function. Not entirely sure about behaviour if
		// multiple threads work on this same function, it seems to work?
		// I.e. pass this same function of same task obj to multiple threads
		// Probably not in keeping with the idea of a single Task() anyway...
		
		// Doing it as virtual do_task and work means that the derived class
		// doesn't have to worry about resource allocation and freeing, the do_task can
		// just actually have the task
		virtual void do_task(int thread_id){};
		
		void work( int thread_id ){
			get_resources();
			do_task( thread_id );
			release_all();
		}
	
};

// Derived class for individual tasks.
// Some issues here regarding what constitutes a task
// Is it just a wrapper for a function? Or should we think it bad
// if we give a single task object to multiple threads? It's reasonable
// to have multiple threads run the same task, but do we need different task objects for that?
// Seems to just be memory overhead: so long as the task is local and/or we assume resources are locked appropriately,
// then it should be fine.
class TaskA : public Task {
	
	public:
		// Manually inherit constructor - though some c++11 stuff can get around this, not read enough
		TaskA( vector<Semaphore*> reqs ) : Task(reqs) {}
		
		// Task here is just to spin cpu for a bit in a loops
		// All variables are local so should play with other stuff to see if can break
		// and/or make non-thread-safe. Probably the main use for semaphore stuff is memory allocation or i/o stuff
		// i.e. if you are going to write to a bit of memory, there is no way you can have multiple threads do that, so would be mutex/binary semaphore
		void do_task(int thread_id ){
			string str = to_string(thread_id);	
			cout<<"Thread #" + str + " got locks."<<endl;
			int x = 0;
			for(int i = 0; i<pow(10,9);i++){
				x++;
			}

			cout<<"Thread #" + str +" output = "<<x<<endl;	
		}	
};
