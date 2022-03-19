/*
 * IVTEntry.h
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#ifndef MARINAFI_IVTENTRY_H_
#define MARINAFI_IVTENTRY_H_

#include "Event.h"

typedef void interrupt (*interrRoutine) (...);
typedef unsigned REG;
const int totalEntries = 256;

class KernelEv;

class IVTEntry {

public:
	IVTEntry (int ivtEntryNo,interrRoutine func);
	~IVTEntry();
	void callOldRoutine();
	void signal();

	KernelEv* myEvent;
	static IVTEntry** tableOfIVTentries;

private:
	int ivtEntryNo;
	interrRoutine oldRoutine;
	interrRoutine newRoutine;
	void prepareEntry();
	void restoreEntry();

};


#endif /* MARINAFI_IVTENTRY_H_ */
