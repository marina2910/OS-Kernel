// File: event.h
#ifndef _event_h_
#define _event_h_

#include "IVTEntry.h"
#include "KernelEv.h"

#define PREPAREENTRY(No,callold)\
	void interrupt inter##No(...);\
	IVTEntry newEntry##No( No, inter##No);\
	void interrupt inter##No(...){\
		if(callold==1)\
			newEntry##No.callOldRoutine();\
		newEntry##No.signal();\
			dispatch();\
	}\



typedef unsigned char IVTNo;
class KernelEv;

class Event {

public:

 Event (IVTNo ivtNo);
 ~Event ();
 void wait ();


protected:

 friend class KernelEv;
 void signal(); // can call KernelEv


private:

 KernelEv* myImpl;

};


#endif
