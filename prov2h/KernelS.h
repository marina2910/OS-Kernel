#ifndef KERNELS_H_
#define KERNELS_H_
#include "Queue.h"
#include "Semaphor.h"


class Queue;

class KernelSem {

public:
 KernelSem (int init=1);
 virtual ~KernelSem ();
 virtual int wait (Time maxTimeToWait);
 virtual int signal(int n=0);
 int val () const; // Returns the current value of the semaphore
 void informMe();

 void block(int time);
 void deblock();

 int value;
 Queue* blocked;
 Queue* blockedByTimer;
 static int ID ;
 int id;

};

void informAboutTime();

class SemElem {
public:
	SemElem(KernelSem* ks) : elem(ks), next(0) {};
	SemElem* next;
	KernelSem* elem;
};

#endif /* KERNELS_H_ */
