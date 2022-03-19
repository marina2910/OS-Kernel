
#include "System.h"
#include "PCB.h"



Idle* idle = 0;
Thread *startingThread = 0;

REG	oldTimerSEG,
	oldTimerOFF;
REG tss,
	tsp,
	tbp;

void tick();


void interrupt timer(...) {

  	if(!PCB::change_on_demand)	{
  		PCB::counter--;
  		asm int 60h;
  		tick();
  		informAboutTime();
  	}

	if (!PCB::change_on_demand && (PCB::counter > 0 || PCB::running->infinite == 1) ) return;

	if(PCB::lockFlag > 0){
  			PCB::change_on_demand = 0;

  #ifndef BCC_BLOCK_IGNORE
  			asm {
  				mov tss, ss
  				mov tsp, sp
  				mov tbp, bp
  			}
  #endif

  			//Saving the current value of sp and ss and bp :)
  			(PCB::running)->mySs = tss;
  			(PCB::running)->mySp = tsp;
  			(PCB::running)->myBp = tbp;


  			//Getting new thread from scheduler and puting the old one in
  			if( ((PCB*)PCB::running)->state == READY  && PCB::running->id != idle->getId()) {
  				Scheduler::put(((PCB*)PCB::running));
  			}

  			(PCB*)(PCB::running) = Scheduler::get();

  			//Check if Scheduler is empty and set running to idle
  			if(PCB::running == 0) {
  				int idleId = idle->getId();
  				PCB* idlePCB = PCB::allThreads->getPCBbyID(idleId);
  				PCB::running = idlePCB;
  			}

  			PCB::counter = ((PCB*)PCB::running)->getTimeSlice();

  			tsp = (PCB::running)->mySp;
  			tss = (PCB::running)->mySs;
  			tbp = (PCB::running)->myBp;

  #ifndef BCC_BLOCK_IGNORE
  			asm {
  				mov ss, tss
  				mov sp, tsp
  				mov bp, tbp
  			}
  #endif
  	  } else PCB::change_on_demand = 1;

  }


  void inic(){
  	asm{
  		cli			// zabranjuju se prekidi
  		push es
  		push ax

  		mov ax,0	// inicijalizuje rutinu za tajmer
  		mov es,ax	// es = 0

  		// pamti staru rutinu
  		// oldTimerSEG = [00072h]; - 1Ch*4h+2h=72h
  		mov ax, word ptr es:0022h
  		mov word ptr oldTimerSEG, ax
  		// oldTimerOFF = [00072h]; - 1Ch*4h=70h
  		mov ax, word ptr es:0020h
  		mov word ptr oldTimerOFF, ax

  		// postavlja novu rutinu
  		mov word ptr es:0022h, seg timer
  		mov word ptr es:0020h, offset timer

  		// postavlja staru rutinu na int 60h
  		mov ax, oldTimerSEG
  		mov word ptr es:0182h, ax
  		mov ax, oldTimerOFF
  		mov word ptr es:0180h, ax

  		pop ax
  		pop es
  		sti			// dozvoljavaju se prekidi
  	}
  }

  void restore(){

  	asm {
  		cli
  		push ax
  		push es

  		mov ax, 0
  		mov es, ax

  		mov ax, word ptr oldTimerSEG
  		mov word ptr es:0022h, ax
  		mov ax, word ptr oldTimerOFF
  		mov word ptr es:0020h, ax

  		pop es
  		pop ax
  		sti
  	}

  }
