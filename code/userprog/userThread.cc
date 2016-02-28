#ifdef CHANGED
#include "userThread.h"
#include "system.h"
Thread *newThread;
int NB_CURRENT=0;
int do_UserThreadCreate(int f, int arg){
  fc_arg *farg=new fc_arg;
  int numThread=Threads->Find();
  if(NB_CURRENT<=NB_THREAD){
    farg->func=f;
    farg->arg=arg;
    farg->numThread=numThread;
    newThread=new Thread("newthread");
    newThread->Fork(StartUserThread,(int)farg);
    NB_CURRENT++;
    delete farg;
    return numThread;
  }
  else{
    return -1;
  }
}
void doUserThreadExit(){
  delete newThread;
  fc_arg *farg=(fc_arg*)currentThread->getArgs();
  int numThread=farg->numThread;

  Threads->Clear(numThread);
  currentThread->Finish();
  NB_CURRENT--;
}
 void StartUserThread(int f){
    fc_arg *farg=(fc_arg*)f;
    int func=farg->func;
    int arg=farg->arg;
    int numThread=farg->numThread;
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();

    machine->WriteRegister(PCReg,func);
    machine->WriteRegister(StackReg, numThread*numPageThread*PageSize - 16);
    machine->WriteRegister(NextPCReg, func + 4);
    machine->WriteRegister(4, arg);
    machine->Run();

    delete farg;
}
#endif
