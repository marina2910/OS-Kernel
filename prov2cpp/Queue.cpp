#include "Queue.h"

#include <stdlib.h>
#include "system.h"


Queue::Queue() {
	lock();

	head = 0;

	unlock();
}


Queue::~Queue() {

	lock();

	Elem* temp;

	while( head != 0 ) {
		temp = head;
		head = head->next;
		delete temp;
	}

	unlock();

}


PCB* Queue::get(){

	PCB* retr;

	lock();

	if(head) {

		Elem* temp = head;
		head = head->next;
		if(!head) {	head->next = 0;	}

		retr = temp->pcb;

		delete temp;

	} else retr = 0;

	unlock();

	return retr;

}

void Queue::remove(PCB* retpcb) {
	lock();

	Elem* prev = 0;
	Elem* cur;
	int found = 0;
	for(cur = head; cur; cur = cur->next) {
		if(cur->pcb == retpcb){	found = 1; break; }
		prev = cur;
	}

	if(!found) {unlock(); return;} //No such pcb in the queue

/*	if(prev != 0) {	prev->next = cur->next;	}
	else {
		head = cur->next;
		head->next = (cur->next)->next;
	}
	cur->next = 0;
	delete cur;*/
	if (cur) {
		if (!prev) head=cur->next;
		else prev->next = cur->next;
		delete cur;
	}
	unlock();
}

void Queue::add(PCB* pcb) {
	lock();

	Elem **current = &head;
	while( (*current) != 0 ){

		if( (*current)->pcb == pcb ) { unlock(); return; }
		current = &((*current)->next);

	}

	(*current) = new Elem(pcb);


	unlock();
}

PCB* Queue::getPCBbyID(ID id) {

	PCB *result = 0;

	lock();

	for (Elem *cur=head; cur; cur=cur->next)
		if (cur->pcb->id == id) {
			result = cur->pcb;
			break;
		}

	unlock();

	return result;
}

PCB* Queue::getByPriority() {
	lock();

	Elem* prev = 0;
	Elem* cur;
	Elem* found;
	Elem* preFound;
	int minID = 10000;

	for(cur = head; cur; cur = cur->next) {
		if(cur->pcb->id < minID){	minID = cur->pcb->id; found = cur; preFound = prev; }
		prev = cur;
	}

	if(!found) {unlock(); return 0;} //No pcb in the queue


	PCB* ret = found->pcb;
	if (found) {
		if (!preFound) head= found->next;
		else preFound->next = found->next;
		delete found;
	}
	unlock();
	return ret;

}
