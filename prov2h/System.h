

#ifndef MARINAFI_SYSTEM_H_
#define MARINAFI_SYSTEM_H_

#include <stdio.h>
#include <stdarg.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "Thread.h"
class PCB;
class Queue;


#define lock() PCB::lockFlag--;
#define unlock() PCB::lockFlag++;

int userMain(int argc, char* argv[]);

void inic();

void restore();

void interrupt timer(...);

class Idle : public Thread{
public:
	void run() {
		while(1);
	}
};


#endif
