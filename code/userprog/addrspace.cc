// addrspace.cc
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

#include <strings.h>		/* for bzero */
#ifdef CHANGED
#include "synchdisk.h"
#include "bitmap.h"
#include "frameprovider.h"
FrameProvider *frameprovider;
static Semaphore *mutex;//Semaphore de protection de donnée partagée
int NumSameSpaceThreads=0;//nombre de threads par addrSpace
static Semaphore  *AllThreadsDone;//semaphore pour l'attente du thread main
static Semaphore  *JoinSemaphore[10];//semaphore pour le join d'un thread utilisateur sur un autre thread utilisateur
static int waiting_join[10];//compte le nombre de join sur un thread
static Semaphore *th;

static void  ReadAtVirtual( OpenFile *executable, int virtualaddr,int numBytes, int position,TranslationEntry *pageTable,unsigned numPages){

  char* Tampon ;

  TranslationEntry *old_pageTable ;
  int old_numPages ;


  Tampon = new char [numBytes] ;
  executable->ReadAt (Tampon, numBytes, position);

  old_pageTable = machine->pageTable ;
  old_numPages = machine->pageTableSize ;

  machine->pageTable=pageTable;
  machine->pageTableSize = numPages;

  int z;
  for(z=0;z<numBytes;z++){
    machine->WriteMem(virtualaddr+z,1,Tampon[z]);
  }

  delete Tampon ;
  machine->pageTable = old_pageTable ;
  machine->pageTableSize = old_numPages ;

}
#endif
//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    #ifdef CHANGED
    mutex = new Semaphore("mutex", 1);
    AllThreadsDone = new Semaphore("AllThreadsDone", 1);
    th = new Semaphore("uniqueThreadCreate", 1);
    int n=0;
    for(n=0;n<10;n++)
    {
      JoinSemaphore[n] = new Semaphore("JoinSemaphore", 0);
      waiting_join[n] = 0;
    }
    frameprovider=new FrameProvider(NumPhysPages);
    #endif
    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    ASSERT (numPages <= NumPhysPages);	// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
	   numPages, size);


      #ifdef CHANGED
      // first, set up the translation
          pageTable = new TranslationEntry[numPages];
              for (i = 0; i < numPages; i++)
                {
                    pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
                    pageTable[i].physicalPage = frameprovider->GetEmptyFrame();
                    pageTable[i].valid = TRUE;
                    pageTable[i].use = FALSE;
                    pageTable[i].dirty = FALSE;
                    pageTable[i].readOnly = FALSE;	// if the code segment was entirely on
                    // a separate page, we could set its
                    // pages to be read-only
                }
      #else
      // first, set up the translation
          pageTable = new TranslationEntry[numPages];
              for (i = 0; i < numPages; i++)
                {
                    pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
                    pageTable[i].physicalPage = i;
                    pageTable[i].valid = TRUE;
                    pageTable[i].use = FALSE;
                    pageTable[i].dirty = FALSE;
                    pageTable[i].readOnly = FALSE;	// if the code segment was entirely on
                    // a separate page, we could set its
                    // pages to be read-only
                }
      #endif

#ifndef CHANGED
// zero out the entire address space, to zero the unitialized data segment
// and the stack segment
    bzero (machine->mainMemory, size);
#endif


// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
		 noffH.code.virtualAddr, noffH.code.size);
     #ifdef CHANGED
            ReadAtVirtual(executable, noffH.code.virtualAddr,noffH.code.size, noffH.code.inFileAddr,pageTable,numPages);
     #else
      	    executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
      			       noffH.code.size, noffH.code.inFileAddr);
     #endif
      }
    if (noffH.initData.size > 0)
      {
    	  DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
    		 noffH.initData.virtualAddr, noffH.initData.size);
         #ifdef CHANGED
                ReadAtVirtual(executable, noffH.initData.virtualAddr,noffH.initData.size, noffH.initData.inFileAddr,pageTable,numPages);
         #else
            	  executable->ReadAt (&
            			      (machine->mainMemory
            			       [noffH.initData.virtualAddr]),
            			      noffH.initData.size, noffH.initData.inFileAddr);
         #endif
      }

}

#ifdef CHANGED
void AddrSpace::addThread(){
  mutex->P();
  NumSameSpaceThreads++;
  if(NumSameSpaceThreads==1){
    AllThreadsDone->P();
  }
  mutex->V();
}
void AddrSpace::removeThread(){
  mutex->P();
  NumSameSpaceThreads--;
  if(NumSameSpaceThreads==0){
     AllThreadsDone->V();
  }
  mutex->V();
}
int AddrSpace::getNumThread(){
  return NumSameSpaceThreads;
}
void AddrSpace::callP(){
  AllThreadsDone->P();
}
void AddrSpace::callJoinP(int numThread){
   waiting_join[numThread]++;
   JoinSemaphore[numThread]->P();
}
void AddrSpace::callJoinV(int numThread){
   while(waiting_join[numThread] > 0)
   {
      JoinSemaphore[numThread]->V();
      waiting_join[numThread]--;
   }
}

void AddrSpace::thP()
{
   th->P();
}

void AddrSpace::thV()
{
   th->V();
}

#endif
//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  #ifdef CHANGED
  unsigned int i=0;
  for(i=0;i<numPages;i++){
    frameprovider->ReleaseFrame(pageTable[i].physicalPage);
  }
  #endif
  // LB: Missing [] for delete
  // delete pageTable;
  delete [] pageTable;
  // End of modification
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to %d\n",
	   numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
