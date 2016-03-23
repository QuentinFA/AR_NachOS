#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg)
{
   readAvail->V();
}

static void WriteDone(int arg)
{
   writeDone->V();
}

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
   readAvail = new Semaphore("read avail", 0);
   writeDone = new Semaphore("write done", 0);

   read = new Semaphore("read", 1);
   write = new Semaphore("write", 1);
   console = new Console(readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
   delete console;
   delete writeDone;
   delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
   read->P();

   console->PutChar(ch);
   writeDone->P();

   read->V();
}

char SynchConsole::SynchGetChar()
{
   read->P();

   readAvail->P();
   char ret=console->GetChar();
   read->V();
   return ret;
}

void SynchConsole::SynchPutString(const char s[])
{
  write->P();
   int i;
   for(i = 0; s[i] != '\0' && i < MAX_STRING_SIZE; i++)
      SynchPutChar(s[i]);
  write->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
  write->P();
   int i;
   for(i = 0; i < n; i++)
      s[i] = SynchGetChar();
  write->V();
}

void SynchConsole::SynchPutInt(int n)
{
   char str[MAX_STRING_SIZE];

   snprintf(str, MAX_STRING_SIZE, "%d", n);
   SynchPutString(str);
}

void SynchConsole::SynchGetInt(int *n)
{
   sscanf(0, "%d", n);
}

#endif
