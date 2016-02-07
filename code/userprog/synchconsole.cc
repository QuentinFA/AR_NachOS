#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console =  new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}
SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}
void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar (ch);
    writeDone->P ();
}

char SynchConsole::SynchGetChar()
{
	  readAvail->P ();	// wait for character to arrive
	  return console->GetChar ();
}

void SynchConsole::SynchPutString(const char s[])
{
	int i=0;
	while(s[i]!='\0' || i < MAX_STRING_SIZE){
		this->SynchPutChar(s[i]);
	    i++;
	}
	synchconsole->SynchPutChar('\n');
}

void SynchConsole::SynchGetString(char *s, int n)
{
	for(int i=0;i<n;i++){
		s[i]=this->SynchGetChar();
	}
	s[n]='\0';
}
#endif // CHANGED