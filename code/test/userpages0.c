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
  int i=0;
  for(i=0;i<4;i++){
   UserThreadCreate(t1, 0);
   UserThreadCreate(t2, 0);
 }

Exit(0);
}

#endif
