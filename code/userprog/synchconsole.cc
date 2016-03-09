#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *r;
static Semaphore *w;

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
   r = new Semaphore("read", 1);
   w = new Semaphore("write", 1);
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
   w->P();
   console->PutChar(ch);
   writeDone->P();
   w->V();
}

char SynchConsole::SynchGetChar()
{
   r->P();
   readAvail->P();
   char c = console->GetChar();
   r->V();
   return c;
}

void SynchConsole::SynchPutString(const char s[])
{
   int i;
   w->P();
   for(i = 0; s[i] != '\0' && i < MAX_STRING_SIZE; i++)
   {
      console->PutChar(s[i]);
      writeDone->P();
   }
   w->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
   int i = 0;
   r->P();

   while(i < n && i < MAX_STRING_SIZE)
   {
      readAvail->P();
      s[i] = console->GetChar();
      i++;
      if(s[i] == EOF || s[i] == '\n')
         break;
   }
   s[i] = '\0';
   r->V();
}

void SynchConsole::SynchPutInt(int n)
{
   char str[MAX_STRING_SIZE];
   int i;

   snprintf(str, MAX_STRING_SIZE, "%d", n);

   w->P();
   for(i = 0; i < (int) strlen(str); i++)
   {
      console->PutChar(str[i]);
      writeDone->P();
   }
   w->V();
}

void SynchConsole::SynchGetInt(int *n)
{
   r->P();
   sscanf(0, "%d", n);
   r->V();
}

#endif
