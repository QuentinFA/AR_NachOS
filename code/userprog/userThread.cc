#ifdef CHANGED
#include "userThread.h"
#include "system.h"
#include "addrspace.h"

Thread *newThread;

int do_UserThreadCreate(int f, int arg){

   currentThread->space->thP();
   int numThread;
     if((numThread = Threads->Find()) != -1){
       fc_arg *farg=new fc_arg;
       farg->func=f;
       farg->arg=arg;
       farg->numThread=numThread;

       snprintf(farg->name, 20, "newthread%d", numThread);

       newThread=new Thread(farg->name);
       newThread->Fork(StartUserThread,(int)farg);
       currentThread->Yield();
       currentThread->space->addThread();
       currentThread->space->thV();
       return numThread;
    }
    else{
      currentThread->space->thV();
      return -1;
   }
}

void do_UserThreadExit(){

  fc_arg *farg=(fc_arg*)currentThread->getArgs();
  int numThread=farg->numThread;

  Threads->Clear(numThread);
  currentThread->space->removeThread();

  currentThread->space->callJoinV(numThread);

  currentThread->Finish();
}

void do_UserThreadJoin(int numThreadAttendu){
   if(Threads->Test(numThreadAttendu))
      currentThread->space->callJoinP(numThreadAttendu);
}

 void StartUserThread(int f){
    fc_arg *farg=(fc_arg*)f;
    int func=farg->func;
    int arg=farg->arg;
    int numThread=farg->numThread;

    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();

    machine->WriteRegister(PCReg,func);
    machine->WriteRegister(StackReg, numThread * numPageThread * PageSize - 16);
    machine->WriteRegister(NextPCReg, func + 4);
    machine->WriteRegister(4, arg);
    machine->Run();
    delete farg;

}

#endif
