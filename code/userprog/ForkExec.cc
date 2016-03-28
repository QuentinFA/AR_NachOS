#ifdef CHANGED
#include "system.h"
#include "addrspace.h"

static void StartUserProcess(int f){
	currentThread->space->InitRegisters ();
	currentThread->space->RestoreState ();
	machine->Run ();
}

int do_ForkExec(char *s){
	OpenFile *executable = fileSystem->Open (s);
	AddrSpace *space;

//	printf("%s\n", s);
	if (executable == NULL){
		printf ("Unable to open file %s\n", s);
		return -1;
	}

	Thread *newThread = new Thread(s);

	if(newThread==NULL){
		printf("Error: Thread not created\n");
		return -1;
	}

	space = new AddrSpace (executable);

	newThread->space = space;
	delete executable;


  addProcess();
	newThread ->ForkExec(StartUserProcess,0);
	currentThread->Yield();

	return 0;
}

void do_ExitProcess(){

}

#endif
