// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
   #include "machine.h"
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}
#ifdef CHANGED
void copyStringFromMachine(int from, char *to, unsigned size)
{

   unsigned i = 0 ;
   int v = 0;

   do
     {
       machine->ReadMem ((int) from+i, 1, &v) ;
       to [i] = (char) v ;
       i = i + 1 ;
     }
   while ((i < size) && (v != 0)) ;
     
   to [i] = 0 ; 

   /*
   unsigned i;
   int v;

   to[size - 1] = '\0';

   for(i = 0; i < size; i++)
   {
      if(machine->ReadMem(((int) from + i), 1, &v))
      {
        to[i] = (char) v;
      }
      else
      {
        //TODO traiter (dans exeptionHandler) les erreurs levées par ReadMem
        DEBUG('u',"Erreur pendant la lecture depuis la machine");
        return;
      }
   }
   */
}

void copyStringToMachine(int to,char* from, unsigned size){
  unsigned nbWr = 0;
  int next;
  for (; nbWr < size -1; nbWr++)
  {
    next = (int) from[nbWr];
    if (next == '\0' || next == EOF || next == '\n'){
      break;
    }
    if (!(machine->WriteMem(to+nbWr,1,next))){
      DEBUG('s',"Erreur pendant l'écriture dans la machine");
      ASSERT(FALSE)
    }
  }
  if (!(machine->WriteMem(to+nbWr,1,'\0'))){
      DEBUG('s',"Erreur pendant l'écriture dans la machine");
      ASSERT(FALSE)
  }
}


#endif
//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    #ifndef CHANGED
    if ((which == SyscallException) && (type == SC_Halt))
      {
	  DEBUG ('a', "Shutdown, initiated by user program.\n");
	  interrupt->Halt ();
      }
    else
      {
	  printf ("Unexpected user mode exception %d %d\n", which, type);
	  ASSERT (FALSE);
      }
   #else
   if (which == SyscallException)
   {
      DEBUG('s'," SC number : %i ",type);

      switch (type)
      {
         case SC_Halt:
         {
            DEBUG('a', "Shutdown, initiated by user program.\n");
            interrupt->Halt();
            break;
         }
         case SC_PutChar:
         {
            synchconsole->SynchPutChar((char) machine->ReadRegister(4));
            break;
         }
         case SC_PutString:
         {
            char s[MAX_STRING_SIZE];
            copyStringFromMachine(machine->ReadRegister(4), s, MAX_STRING_SIZE);
            synchconsole->SynchPutString(s);
            break;

         }
         case SC_GetChar:
         {
            int r = (int) synchconsole->SynchGetChar();
            machine->WriteRegister(2, r);
            break;
         }
         case SC_GetString:
         {
            int sizeMips = machine->ReadRegister(4);
            int size = machine->ReadRegister(5);
            char *str = new char[MAX_STRING_SIZE];

            synchconsole->SynchGetString(str, size);
            copyStringToMachine(sizeMips, str, size);

            delete[] str;

            break;
         }
         case SC_GetInt:
         {
            int i;
            int valide;
            int at = machine->ReadRegister(4);
            //i = synchconsole->SynchGetInt();
            valide = synchconsole->SynchGetInt(&i);
            machine->WriteMem(at, sizeof(int), i);
            machine->WriteRegister(2,valide);
            break;
         }
         case SC_PutInt:
         {
            synchconsole->SynchPutInt(machine->ReadRegister(4));
            break;
         }
         default:
         {
            printf("Unexpected user mode exception %d %d\n", which, type);
            ASSERT(FALSE);
         }
      }
   }
   #endif
       // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}

