
#include "Thread.h"
#include "Schedule.h"
#include "PCB.h"
#include "System.h"
#include "IVTEntry.h"
#include <stdio.h>
#include <iostream.h>
#include <dos.h>
#include "Queue.h"


extern Idle* idle;
extern Thread *startingThread;

int main (int argc, char *argv[]) {

	startingThread = new Thread(200, 1); //stack size is not important, we are using main's stack

	int startingId = startingThread->getId();
	PCB* startingPCB = PCB::allThreads->getPCBbyID(startingId);
	startingPCB->state = READY;
	PCB::running = startingPCB;

	idle = new Idle();
	int idleId = idle->getId();
	PCB* idlePCB = PCB::allThreads->getPCBbyID(idleId);
	idlePCB->state = READY;

	//Initialize the system by moving interrupts
	inic();

	int result = userMain(argc, argv);

	//Restore the old system state
	restore();

	//delete the idle, starting thread context and queue of allThreads
	delete idle;
	delete startingThread;
	delete PCB::allThreads;
	delete[] IVTEntry::tableOfIVTentries;
	//return the result from user main
	return result;
}
