#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg)
{
   DEBUG('s',"=======>readAvail\n");
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
   console->PutChar(ch);
   writeDone->P();
}

char SynchConsole::SynchGetChar()
{
   DEBUG('s'," ====>SynchGetChar \n");
   readAvail->P();
   return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
   int i;
   for(i = 0; i < MAX_STRING_SIZE; i++)
   {
      console->PutChar(s[i]);
      writeDone->P();
      if(s[i] == '\0')
         break;
   }
   DEBUG('s',"fin SynchPutString\n");
}

void SynchConsole::SynchGetString(char *s, int n)
{
   int i = 0;

   while(i < n && i < MAX_STRING_SIZE)
   {
      readAvail->P();
      DEBUG('s',"avant GetChar %i \n", i );
      s[i] = console->GetChar();
      DEBUG('s',"==>apres getchar %i : %i\n", i, s[i]);
      if(s[i] == EOF || s[i] == '\n'){
         break;
      }
      i++;
   }
   s[i] = '\0';
}

void SynchConsole::SynchPutInt(int n)
{
   char str[MAX_STRING_SIZE];
   int i;

   snprintf(str, MAX_STRING_SIZE, "%d", n);

   for(i = 0; i < (int) strlen(str); i++)
   {
      console->PutChar(str[i]);
      writeDone->P();
   }
}

int SynchConsole::SynchGetInt(int* res)
{
   //char str[MAX_STRING_SIZE];
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
