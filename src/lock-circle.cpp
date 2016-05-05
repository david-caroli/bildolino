#include "lock-circle.hpp"





void lcInitLockCircle(LockCircle &lc, unsigned int locks) {
	lc.threadAInitDone = false;
	lc.threadBInitDone = false;
	lc.n = locks;
	if(lc.n < 2)
		lc.n = 2;
	lc.locks = new std::mutex[lc.n];
}

void lcThreadInit(LockCircle &lc, bool isThreadA, unsigned int startLock) {
	lc.locks[startLock].lock();
	lc.initLock.lock();
	if(isThreadA) {
		lc.threadAInitDone = true;
	} else {
		lc.threadBInitDone = true;
	}
	lc.initLock.unlock();
}

void lcWaitForInit(LockCircle &lc) {
	lc.initLock.lock();
	while(!lc.threadAInitDone || !lc.threadBInitDone) {
		lc.initLock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		lc.initLock.lock();
	}
	lc.initLock.unlock();
}

void lcGetNextLock(LockCircle &lc, unsigned int &currentLock) {
	unsigned int nextLock = (currentLock+1) % lc.n;
	lc.locks[nextLock].lock();
	lc.locks[currentLock].unlock();
	currentLock = nextLock;
}

void lcUnlock(LockCircle &lc, unsigned int &currentLock) {
	lc.locks[currentLock].unlock();
}

void lcCleanup(LockCircle &lc) {
	delete[] lc.locks;
}
