#ifdef CHANGED

#include "syscall.h"

void print(char* c, int n)
{
   int i;
   for (i = 0; i < n; i++)
   {
      PutString(c);
      PutChar('\n');
   }
   PutChar('\n');
}

int main()
{
   print("Hey !",4);
   Halt();
}

#endif
