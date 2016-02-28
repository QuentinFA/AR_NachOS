#include "syscall.h"

void newThread(void * arg) {
    PutString("test\n");
    UserThreadExit();
}

int main() {
    PutString("Début du main\n");
    UserThreadCreate(newThread, 0);


}
