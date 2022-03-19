/*
 * KernelEv.h
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#ifndef MARINAFI_KERNELEV_H_
#define MARINAFI_KERNELEV_H_
#include "PCB.h"
#include "KernelS.h"
#include "IVTEntry.h"

typedef unsigned char IVTNo;

class KernelEv{

public:

	KernelEv (IVTNo ivtno);
	~KernelEv();
	void signal();
	void wait();

private:
	PCB* myOwner;
	KernelSem* mySem;
	IVTNo ivtNo;
	int value;
};



#endif /* MARINAFI_KERNELEV_H_ */
