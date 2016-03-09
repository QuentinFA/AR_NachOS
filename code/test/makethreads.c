#ifdef CHANGED
#include "syscall.h"


void newThread1(void * arg) {
//    PutString("fini d'attendre\n");

//UserThreadJoin(1,3);
    PutString("Thread 1\n");
    UserThreadExit();
}
void newThread2(void * arg) {

    PutString("Thread 2\n");
    UserThreadExit();
}
void newThread3(void * arg) {
    PutString("Thread 3\n");
    UserThreadExit();
}
void newThread4(void * arg) {
    PutString("Thread 4\n");
    UserThreadExit();
}
void newThread5(void * arg) {
    PutString("Thread 5\n");
    UserThreadExit();
}
void newThread6(void * arg) {
    PutString("Thread 6\n");
    UserThreadExit();
}
void newThread7(void * arg) {
    PutString("Thread 7\n");
    UserThreadExit();
}
void newThread8(void * arg) {
    PutString("Thread 8\n");
    UserThreadExit();
}
void newThread9(void * arg) {
    PutString("Thread 9\n");
    UserThreadExit();
}
void newThread10(void * arg) {
    PutString("Thread 10\n");
    UserThreadExit();
}
void newThread11(void * arg) {
    PutString("Thread 11\n");
    UserThreadExit();
}
void newThread12(void * arg) {
    PutString("Thread 12\n");
    UserThreadExit();
}

int main() {

    UserThreadCreate(newThread1, 0);
    UserThreadCreate(newThread2, 0);
    UserThreadCreate(newThread3, 0);
    UserThreadCreate(newThread4, 0);
  //  UserThreadCreate(newThread5, 0);
/*    PutString("BonSoir\n");
    UserThreadCreate(newThread6, 0);
    UserThreadCreate(newThread7, 0);
    UserThreadCreate(newThread8, 0);
    UserThreadCreate(newThread9, 0);
    UserThreadCreate(newThread10, 0);
    //UserThreadCreate(newThread11, 0);
    //UserThreadCreate(newThread12, 0);
*/
    Halt();
}
#endif
