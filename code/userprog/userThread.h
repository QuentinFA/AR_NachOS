#ifdef CHANGED

typedef struct fc_arg fc_arg;
struct fc_arg{
  int func;
  int arg;
  int numThread;
};
extern int do_UserThreadCreate(int f, int arg);
extern void doUserThreadExit();
int UserThreadCreate(void f(void *arg), void *arg);
void StartUserThread(int f);
#endif
