#ifdef CHANGED

#include "syscall.h"

int main()
{
   int i = 420;
   int res;
   PutInt(i);
   PutChar('\n');
   res = GetInt(&i);
   if(!res)
   		PutString("YEAH MADAFAKA\n");
   PutInt(i);
   PutChar('a');
   Halt();
}

#endif
