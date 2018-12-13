# C++ Task-based Multi-threading Library

A simple attempt at a low-ish level library of functions/classes to enable task-based concurrency and parallelism.

### Current functionality

#### Semaphore class
Used to protect resources which can have multiple simultaneous threads accessing it

#### Task
Parent class from which user task classes are derived. Deals with all of the resource allocation and freeing, user is just required to overwrite the virtual 'work()' method, e.g. TaskA()

### Future work
- Make more versatile using template for wuser defined work functions
- Thread priority - develop priority queue system for tasks
- Dynamic task scheduliing - develop scheduler to deal with when there are more tasks than threads, and to free up idle threads if their tasks are stuck for one whos resources are available

The last two may be straying into OS territory so possibly useless/wasteful
