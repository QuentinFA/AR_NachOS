#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static Semaphore *lecture;
static Semaphore *lectureAll;
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

   lecture = new Semaphore("lecture", 1);
   lectureAll = new Semaphore("lectureAll", 1);
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
   lecture->P();

   console->PutChar(ch);
   writeDone->P();

   lecture->V();
}

char SynchConsole::SynchGetChar()
{
   lecture->P();

   readAvail->P();
   char ret=console->GetChar();
   lecture->V();
   return ret;
}

void SynchConsole::SynchPutString(const char s[])
{
  lectureAll->P();
   int i;
   for(i = 0; s[i] != '\0' && i < MAX_STRING_SIZE; i++)
      SynchPutChar(s[i]);
  lectureAll->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
  lectureAll->P();
   int i;
   for(i = 0; i < n; i++)
      s[i] = SynchGetChar();
  lectureAll->V();
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
