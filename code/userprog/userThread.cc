#ifdef CHANGED
#include "userThread.h"
#include "system.h"
#include "addrspace.h"

Thread *newThread;
int NB_CURRENT=0;
int numThreadAttenduLocale=-1;

// TODO NB_CURRENT Becomes useless by using a BitMap
int do_UserThreadCreate(int f, int arg){
   int numThread;
     if((numThread = Threads->Find()) != -1){
       fc_arg *farg=new fc_arg;
       farg->func=f;
       farg->arg=arg;
       farg->numThread=numThread;
       newThread=new Thread("newthread");
       newThread->Fork(StartUserThread,(int)farg);
       currentThread->Yield();
      currentThread->space->addThread();
       delete farg;
       return numThread;
    }
    else{
      return -1;
   }
}

void do_UserThreadExit(){

  fc_arg *farg=(fc_arg*)currentThread->getArgs();
  int numThread=farg->numThread;

  Threads->Clear(numThread);
  currentThread->space->removeThread();

  if(numThreadAttenduLocale!=-1){
  currentThread->space->callJoinV(numThread);
  numThreadAttenduLocale=-1;
}

  currentThread->Finish();
}
void do_UserThreadJoin(int numThreadAttendu){
  numThreadAttenduLocale=numThreadAttendu;
  fc_arg *farg=(fc_arg*)currentThread->getArgs();
  int numThread=farg->numThread;

currentThread->space->callJoinP(numThread);


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
