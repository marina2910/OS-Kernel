#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Pcb.h"

typedef int ID;
class Elem {

public:
	Elem(PCB* pcb) {
		this->pcb = pcb;
		this->next = 0;
	}
	PCB* pcb;
	Elem* next;
};

class Queue {

 public:
	void add(PCB* pcb);
	PCB* get();
	void remove(PCB* pcb);

	Queue();
	~Queue();

	PCB* getPCBbyID(ID id);
	PCB* getByPriority();


	Elem* head;
//	Elem* tail;
};


#endif
