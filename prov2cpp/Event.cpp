/*
 * Event.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#include "Event.h"


Event::Event(IVTNo ivtNo) {
	myImpl = new KernelEv(ivtNo);
}

void Event::signal(){
	myImpl->signal();
}

void Event::wait() {
	myImpl->wait();
}

Event::~Event(){
	delete myImpl;
}
