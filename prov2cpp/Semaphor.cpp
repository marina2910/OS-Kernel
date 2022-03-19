/*
 * Semaphor.cpp
 *
 *  Created on: Aug 6, 2020
 *      Author: OS1
 */
#include "Semaphor.h"
#include "KernelS.h"


Semaphore::Semaphore (int init) {
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore() {
	delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait) {
	return this->myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
	return this->myImpl->signal();
}

int Semaphore::val () const{
	return this->myImpl->val();
}
