
/*
 * KernelS.cpp
 *
 *  Created on: Aug 6, 2020
 *      Author: OS1
 */

#include "KernelS.h"
#include "System.h"

SemElem* head;
SemElem* tail;
int KernelSem::ID = 0;


 KernelSem::KernelSem (int init) {
	 lock();
	 this->value = init;
	 this->blocked = new Queue();
	 this->blockedByTimer = new Queue();
	 this->id = ID++;
//----------Adding to the list-----------------//
	 SemElem* add = new SemElem(this);
	 if(head == 0) {
		 head = add;
		 tail = add;
		 head->next = 0;
		 tail->next = 0;
	 } else {
		 tail->next = add;
		 tail = add;
		 add->next = 0;
	 }
	 unlock();
 }

 KernelSem::~KernelSem (){
lock();
//--------Removing from the list--------------//
	 SemElem* cur = head;
	 SemElem* prev = 0;
	 while(cur) {
		 if(cur->elem->id == this->id) { break; }
		 prev = cur;
		 cur = cur->next;
	 }
	if( !prev ) {

		if(head == tail) {
			tail = 0;
			head = 0;
			tail->next = 0;
			head->next = 0;
		} else head = head->next;

	} else { prev->next = cur->next; }

	cur->next = 0;

	 delete blocked;
	 delete blockedByTimer;
	 unlock();
 }

 void KernelSem::block(int time){
	 lock();
	 PCB::running->timeToWaitOnSem = time;
	 PCB::running->timePassed = 0;

	 if(time > 0) {	 this->blockedByTimer->add((PCB*)PCB::running);	 }
	 else {	 this->blocked->add((PCB*)PCB::running); }
	 //Rows of blocked on this semaphore


	 PCB::running->state = BLOCKED;

	 unlock();

	 dispatch();
 }


 void KernelSem::deblock() {

	 lock();

	PCB* toBeDeblocked;
	if(blocked->head != 0){	toBeDeblocked = this->blocked->get();	}
	else if(blockedByTimer->head != 0){ toBeDeblocked = this->blockedByTimer->get(); }
	else return;

	toBeDeblocked->deblockedManualy = 1;
	toBeDeblocked->state = READY;

	 Scheduler::put(toBeDeblocked);

	 unlock();

 }


 int KernelSem::wait (Time maxTimeToWait){

lock();
	 if(--value < 0) {
		 unlock();
		 block(maxTimeToWait);
	 } else{
		 unlock();
		 PCB::running->deblockedManualy = 1;
	 }

	 return PCB::running->deblockedManualy;

 }


 int KernelSem::signal(int n) {

lock();
	 int ret = 0; //return value of function signaL
	 if(n<0) ret = n; //Illegal argument value

	 int newVal;
	 int increment = n;
	 if(n==0) increment++; //if n == 0, increment will be one, otherwise it is n
	 newVal = value + increment;


	 if(value < 0) {
		 while((increment-- > 0) && (blocked->head != 0 || blockedByTimer->head != 0))
		 {
			 deblock();
		 	 ret++;
		 }
		 // Deblock them as long as there are blocked threads and the increment is greater than zero
	 }

	 value = newVal; //set the new value of the semaphore

unlock();

//	dispatch();
	 return ret;
 }


 int KernelSem::val () const {
	 return value;
 }


 void KernelSem::informMe() {
lock();
	 if(this->blockedByTimer->head == 0){ unlock(); return; } //nothing to inform

	 Elem* cur = blockedByTimer->head;
	 Elem* prev = 0;

	 for( ; cur; ) {

		 if( ( (++(cur->pcb->timePassed)) - cur->pcb->timeToWaitOnSem) == 0 ) {

			 //deblock the thread
			 cur->pcb->deblockedManualy = 0;
			 cur->pcb->state = READY;
			 Scheduler::put(cur->pcb);
			 value++;

			 //remove the element from the list of blockedByTimer
			 Elem* temp = cur;
			 cur = cur->next;
			 blockedByTimer->remove(temp->pcb);

		 } else {
			 prev = cur;
			 cur = cur->next;
		 }
	 }
unlock();
 }





void informAboutTime() {
lock();
	SemElem* cur = head;
	while(cur) {
		cur->elem->informMe();
		cur = cur->next;
	}
unlock();
}

