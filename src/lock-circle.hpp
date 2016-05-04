#ifndef LOCK_CIRCLE_HPP
#define LOCK_CIRCLE_HPP

/*
 * Guarantees for two threads that they will "run into" a locked lock after finishing their work.
 * There are n locks ordered in a thought circle. Before releasing a lock, each thread will first aquire the next lock in the circle.
 * By always retaining ownership of at least one lock each thread will stop the other thread from overtaking, making active waiting unnecessary.
 * If you don't use at least one more lock than threads, a deadlock will be created.
 * This system is only intended for two threads, as using more threads is prone to creating a bottleneck.
 */


#include <thread>
#include <mutex>
#include <chrono>


typedef struct {
	std::mutex initLock;
	bool threadAInitDone;
	bool threadBInitDone;
	std::mutex *locks;
	unsigned int n;
} LockCircle;



// sets all initial values for members of lc (call this first!)
void lcInitLockCircle(LockCircle &lc, unsigned int locks);
// aquires the starting lock and sets a ready value in lc
void lcThreadInit(LockCircle &lc, bool isThreadA, unsigned int startLock);
// actively waits until the both threads have called lcThreadInit
void lcWaitForInit(LockCircle &lc);
// aquires the next lock, then releases the current lock
// this changes the value of the int reference currentLock
void lcGetNextLock(LockCircle &lc, unsigned int &currentLock);


#endif // LOCK_CIRCLE_HPP
