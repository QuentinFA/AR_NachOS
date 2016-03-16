#ifdef CHANGED
   #ifndef _USERTHREAD_H
   #define _USERTHREAD_H
   typedef struct fc_arg fc_arg;
   struct fc_arg{
     int func;
     int arg;
     int numThread;
   };
   int do_UserThreadCreate(int f, int arg);
   void do_UserThreadExit();
   void do_UserThreadJoin(int numThreadAttendu);
   void StartUserThread(int f);

   #endif
#endif
