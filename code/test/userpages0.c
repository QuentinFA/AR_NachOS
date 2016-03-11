#ifdef CHANGED
#include "syscall.h"

void t1(void *arg)
{
   int i;
   for(i = 0; i < 26; i++)
   {
      PutChar(i + 'a');
   }
   PutChar('\n');
   UserThreadExit();
}

void t2(void *arg)
{
   int j;
   for(j = 0; j < 26; j++)
   {
      PutChar('Z' - j);
   }
   PutChar('\n');
   UserThreadExit();
}

int main()
{
   UserThreadCreate(t1, 0);
   UserThreadCreate(t2, 0);
   Halt();
}


#endif


