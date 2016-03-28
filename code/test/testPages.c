#include "syscall.h"

int main()
{
  int i=0;
  for(i=0;i<6;i++){
  ForkExec("build/userpages1");
  ForkExec("build/userpages0");
}
  Exit(0);
}
