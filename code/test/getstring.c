#ifdef CHANGED

#include "syscall.h"

int main()
{
   char c[11];

   GetString(c, 10);
   PutString(c);
   Halt();
}

#endif
