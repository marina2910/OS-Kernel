/*
 * PCB.h
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "Schedule.h"
#include "KernelS.h"
#include "Queue.h"
#include "Thread.h"

class Queue;
typedef unsigned REG;
enum STATE {NEW = 0, READY = 1, BLOCKED = 2, BLOCKME = 3, FINISHED = 4};

class PCB {
public:
	static ID idCnt;
	static PCB* running;
	static Queue* allThreads;

	static int change_on_demand;
	static int lockFlag;
	static int counter;

	Queue *waitToCompleteQueue;

	volatile STATE state;
	volatile int deblockedManualy;
	volatile int timeToWaitOnSem;
	volatile int timePassed;


	int infinite; //indicates if thread can run infinitely

	REG mySp,
		cs,
		ip,
		mySs,
		myBp,
		ds;

	ID id;

	PCB(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice, Thread* myThread);
	ID getPCBId();
	Thread* getMyThread();
	Time getTimeSlice();
	void setFinished();
	int isFinished();
	~PCB();
	friend void inic();
	friend void restore();


private:
	Thread* myThread;
	StackSize stackSize;
	Time timeSlice;
	REG*  stack;

	static void body ();
};



#endif /* PCB_H_ */
