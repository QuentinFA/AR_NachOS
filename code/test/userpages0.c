#ifdef CHANGED
#include "syscall.h"

void t1(void *arg)
{
   int i;
   for(i = 0; i < 26; i++)
   {
      PutChar(i + 'a');
   }
   UserThreadExit();
}

void t2(void *arg)
{
   int j;
   for(j = 0; j < 26; j++)
   {
      PutChar('Z' - j);
   }
   UserThreadExit();
}

int main()
{
   int pid1;
   int pid2;
   pid1 = UserThreadCreate(t1, 0);
   pid2 = UserThreadCreate(t2, 0);
   UserThreadJoin(pid1);
   UserThreadJoin(pid2);
   PutChar('\n');
   Halt();
}

#endif
