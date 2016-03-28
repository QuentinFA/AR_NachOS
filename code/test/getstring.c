#ifdef CHANGED

#include "syscall.h"

int main()
{
   char c[11];
   char c2[11];
   PutString("this is a string\n");

   GetString(c, 10);
   PutString (c) ;
   PutString("\n");
   GetString(c2, 10);

   PutString(c2) ;
   PutString("\n");
   Halt();
}

#endif
