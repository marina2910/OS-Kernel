/*
 * Thread.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */
#include "Thread.h"
#include"PCB.h"
#include <stdio.h>
#include "system.h"
#include "Queue.h"
#include "Schedule.h"
unsigned TBP, TSS, TSP;

extern Idle* idle;
extern Thread *startingThread;


Thread::Thread (StackSize stackSize, Time timeSlice) {
	lock();
	this->myPCB = new PCB(stackSize, timeSlice, this);
	unlock();
}

Thread::~Thread() {
	waitToComplete();
	lock();
	delete myPCB;
	unlock();
}

void Thread::start() {
	lock();
	Scheduler::put(myPCB);
	myPCB->state = READY;
	unlock();
}

ID Thread::getId() {

	return myPCB->getPCBId();

}
ID Thread::getRunningId(){

	return ((PCB*)PCB::running)->getPCBId();

}
Thread* Thread::getThreadById(ID id) {

	return PCB::allThreads->getPCBbyID(id)->getMyThread();

}

void Thread::waitToComplete() {

	lock();

	if (myPCB->state == FINISHED || myPCB->state == NEW) { unlock(); return; }

	if (myPCB == PCB::running) { unlock(); return; }

	if (this == startingThread) { unlock(); return; } //No thread should wait system threads to finish
	if (this == idle) { unlock(); return; }

	PCB::running->state = BLOCKED;
	myPCB->waitToCompleteQueue->add(PCB::running);


	unlock();
	dispatch();

}

void dispatch () {

	PCB::change_on_demand = 1;
	asm int 08h;
}

