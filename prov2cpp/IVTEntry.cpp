/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */
#include "IVTEntry.h"
#include <dos.h>

REG oldSeg, oldOff, newSeg, newOff, newint;
IVTEntry** IVTEntry::tableOfIVTentries = new IVTEntry* [256];

IVTEntry::IVTEntry(int ivtEntryNo,interrRoutine func) {

	this->ivtEntryNo = ivtEntryNo;
	this->newRoutine = func;
	this->myEvent = 0;
	tableOfIVTentries[ivtEntryNo] = this;
	prepareEntry();

}

IVTEntry::~IVTEntry(){
	tableOfIVTentries[ivtEntryNo] = 0;
	if(myEvent) delete myEvent;
	restoreEntry();
}

void IVTEntry::callOldRoutine() {
	oldRoutine();
}

void IVTEntry::signal() {
	if(myEvent) myEvent->signal(); //signal the KernelEvent
}



void IVTEntry::prepareEntry(){


#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtEntryNo);
	setvect(ivtEntryNo, newRoutine);
#endif

};

void IVTEntry::restoreEntry() {

#ifndef BCC_BLOCK_IGNORE
	setvect(ivtEntryNo, oldRoutine);
#endif

}
