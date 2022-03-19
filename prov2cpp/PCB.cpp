/*
 * PCB.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */
#include "PCB.h"
#include <dos.h>

#include "Queue.h"
#include "Schedule.h"
#include "Thread.h"
#include "System.h"

ID PCB::idCnt = 0;
Queue* PCB::allThreads = new Queue();
int PCB::change_on_demand = 0;
PCB* PCB::running = 0;
int PCB::lockFlag = 1;
int PCB::counter = 1;




PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread){
	if(stackSize >= 65535) this->stackSize = 65535;
	else this->stackSize = stackSize;

	stackSize /= sizeof(REG);

	this->timeSlice = timeSlice;
	this->myThread = myThread;
	this->id = idCnt++;

	this->state = NEW;
	this->timePassed = 0;
	this->timeToWaitOnSem = 0;
	this->deblockedManualy = 1;

	PCB::allThreads->add(this);
	if(!timeSlice) infinite = 1;
	else infinite = 0;

	waitToCompleteQueue = new Queue();

	stack = new REG[stackSize]; //Allocate memory for stack

#ifndef BCC_BLOCK_IGNORE
	this->cs = FP_SEG(PCB::body);
	this->ip = FP_OFF(PCB::body);
	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2 ] = this->cs;
	stack[stackSize - 3 ] = this->ip;
	this->mySs = FP_SEG(stack + stackSize - 12);
	this->mySp = FP_OFF(stack + stackSize - 12);
	this->myBp = FP_OFF(stack + stackSize - 12);
#endif
}


PCB::~PCB() {
	delete stack;
	delete waitToCompleteQueue;
}

ID PCB::getPCBId() {
	return id;
}

Thread* PCB::getMyThread() {
	return myThread;
}


void PCB::body () {
	PCB::running->myThread->run();

	lock();

	((PCB*)PCB::running)->setFinished();
	PCB *blocked = PCB::running->waitToCompleteQueue->get();

	while (blocked) {
		blocked->state = READY;
		Scheduler::put(blocked);
		blocked = PCB::running->waitToCompleteQueue->get();
	}
	unlock();

	dispatch();
}


Time PCB::getTimeSlice() {
	return timeSlice;
}

void PCB::setFinished(){
	state = FINISHED;
}

int PCB::isFinished() {
	return state;
}
