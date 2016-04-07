#include "shell.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
extern void Copy (const char *unixFile, const char *nachosFile);
extern void Print (char *file), PerformanceTest (void);
extern void createDir(char *name);
void do_shell(){
  #ifdef FILESYS

for (;;)
 {

    string cmd;
    cout << ">";
    std::getline (std::cin,cmd);
    char tab2[1024];
    strcpy(tab2, cmd.c_str());

    char * pch=new char();
    pch = strtok (tab2," ");


    char * cmd1=new char();
    char * arg1=new char();
    char * arg2=new char();

    cmd1=pch;
    pch = strtok (NULL, " ");
    arg1=pch;
    pch = strtok (NULL, " ");
    arg2=pch;
    pch = strtok (NULL, " ");

if(cmd1!=NULL){


            if (!strcmp (cmd1, "-cp")){
                  ASSERT (arg1!=NULL && arg2!=NULL);
                  Copy (arg1, arg2);
              }
              if (!strcmp (cmd1, "-l")){
                  fileSystem->List ();
              }
              if (!strcmp (cmd1, "-D")){
                  fileSystem->Print ();
              }
              if (!strcmp (cmd1, "-q")){
                  interrupt->Halt ();
              }
              if (!strcmp (cmd1, "-p"))
                {			// print a Nachos file
              ASSERT (arg1 !=NULL );
                      Print (arg1);
                }
              if (!strcmp (cmd1, "-r"))
                {			// remove Nachos file
              ASSERT (arg1 !=NULL);
                fileSystem->Remove (arg1);
              }
              if (!strcmp (cmd1, "-t"))
                {			// performance test
              PerformanceTest ();
                }
                if (!strcmp (cmd1, "mkdir"))
                {			// create a directory
                  ASSERT (arg1 !=NULL );
                    fileSystem->createDirectory(arg1);
                }
                if (!strcmp (cmd1, "rm"))
                {			// create a directory
                  ASSERT (arg1 !=NULL );
                    fileSystem->Remove(arg1);
                }
                if (!strcmp (cmd1, "cd"))
                {			// create a directory
                  ASSERT (arg1 !=NULL );
                    fileSystem->ChangeDir(arg1);
                }

  }
}
  #endif
}
