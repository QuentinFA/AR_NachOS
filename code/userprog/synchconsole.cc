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
   for(i = 0; i < MAX_STRING_SIZE; i++)
   {
      console->PutChar(s[i]);
      writeDone->P();
      if(s[i] == '\0')
         break;
   }

  write->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
  write->P();
  int i = 0;

   while(i < n && i < MAX_STRING_SIZE)
   {
      readAvail->P();
      DEBUG('s',"avant GetChar %i \n", i );
      s[i] = console->GetChar();
      DEBUG('s',"==>apres getchar %i : %i\n", i, s[i]);
      if(s[i] == EOF || s[i] == '\n')
         break;
      i++;
   }
   s[i] = '\0';

  write->V();
}

void SynchConsole::SynchPutInt(int n)
{
   char str[MAX_STRING_SIZE];
   int i;

   snprintf(str, MAX_STRING_SIZE, "%d", n);

   write->P();
   for(i = 0; i < (int) strlen(str); i++)
   {
      console->PutChar(str[i]);
      writeDone->P();
   }
   write->V();

}

int SynchConsole::SynchGetInt(int* res)
{
   // TODO Use of read Semephore
   char c;
   int n = 0;
   int valide = 1;
   while(true)
   {
      readAvail->P();
      c = console->GetChar();
      if(c == EOF || c == '\n')
         break;
      if( valide && (0 >= c - '0' || 9 < c-'0'))
         valide = 0;
      n = 10*n + (c - '0');
   }
   *res = n;
   return valide;
}

#endif
