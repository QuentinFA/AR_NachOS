#ifdef CHANGED

#include "syscall.h"

int main()
{
   char c;

   do {
      c = GetChar();
      PutChar(c);
   } while(c != '\n');
   Halt();
}

#endif
