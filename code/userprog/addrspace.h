// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#ifdef CHANGED
#include "bitmap.h"
#include "synch.h"
#endif


#define UserStackSize		1024 * 26 	// increase this as necessary!
class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space
    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch
    #ifdef CHANGED
    void addThread();//ajoute un au compteur quand un thread est créé et prend un semaphore quand c'est le premier thread creer
    void removeThread();//retire 1 du compteur quand le thread est détruit et libere le semaphore quand c'est le dernier thread détruit
    int getNumThread();//renvoie le nombre de thread.
    void callP();//appel du semaphore pour bloquer le thread principal tant que le tous les threads n'ont pas fini de d'exécuter
    void callJoinP(int numThread);//semaphore pour le UserThreadJoin
    void callJoinV(int numThread);//semaphore pour le UserThreadJoin
    void thP();
    void thV();
    BitMap *Threads;

    #endif
  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual
    // address space
    #ifdef CHANGED
    int NumSameSpaceThreads;//nombre de threads par addrSpace

    Semaphore *mutex;//Semaphore de protection de donnée partagée
    Semaphore  *AllThreadsDone;//semaphore pour l'attente du thread main
    Semaphore  *JoinSemaphore[10];//semaphore pour le join d'un thread utilisateur sur un autre thread utilisateur
    int waiting_join[10];//compte le nombre de join sur un thread
    Semaphore *th;
    #endif

};

#endif // ADDRSPACE_H
