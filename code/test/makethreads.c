#ifdef CHANGED
#include "syscall.h"

int cenyo = 0;

void newThread(void * arg) {
    PutString("Hey !\n");
    cenyo++;
    UserThreadExit();
}

int main() {
    PutString("Début du main\n");
    UserThreadCreate(newThread, 0);
    while(!cenyo)
      ;
    Halt();
}
#endif
