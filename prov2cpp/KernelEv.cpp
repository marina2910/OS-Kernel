/*
 * KernelEv.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */


#include "KernelEv.h"
#include "System.h"

KernelEv::KernelEv(IVTNo ivtno) {
	lock();
	this->ivtNo = ivtno;
	this->myOwner = (PCB*)PCB::running;
	this->mySem = new KernelSem(0);
	this->value = 1;
	IVTEntry::tableOfIVTentries[ivtno]->myEvent = this;
	unlock();
}

KernelEv::~KernelEv() {
	lock();
	IVTEntry::tableOfIVTentries[ivtNo]->myEvent = 0;
	delete mySem;
	unlock();
}



void KernelEv::signal(){
	int val = mySem->val();
	if(val < 1) mySem->signal(); //binarny semaphore
}

void KernelEv::wait() {
	if((PCB*)PCB::running != myOwner) return; //no effect
	mySem->wait(0);
}

