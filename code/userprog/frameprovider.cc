#ifdef CHANGED
//#include "frameprovider.h"
#include "system.h"
#include "synch.h"
static Semaphore *mutex;
    FrameProvider::FrameProvider(int nombrePages){
      map=new BitMap(nombrePages);
      mutex = new Semaphore("mutex", 1);
      Indice=0;
       numfois=0;
    }

    FrameProvider::~FrameProvider(){
      delete map;
    }

    int FrameProvider::GetEmptyFrame(){
        mutex->P();
        if(NumAvailFrame()==0)
        ASSERT("No frame available");

      //  int pageToAloccate= GetFirstFit(Indice);
        int pageToAloccate= GetRandomFit();
        DEBUG('y', "Getting frame number  %d \n",pageToAloccate);
        bzero(&(machine->mainMemory[ PageSize * pageToAloccate ] ), PageSize );
        mutex->V();
        return pageToAloccate;
    }

    void FrameProvider::ReleaseFrame(int numPage){
  //    printf("Frame %d released\n",numPage );
  DEBUG('y', "releasing frame number  %d \n",numPage);
        map->Clear(numPage);
    }

    int FrameProvider::NumAvailFrame(){
      return map->NumClear();
    }

    int FrameProvider::GetRandomFit(){
      int pageToAloccate=0;

    //  do{
        pageToAloccate=Random()%NumPhysPages;
        //Une erreure stupide. Politique2 exécute la methode find() qui marque
        //automatiquement le bit. donc le test est toujours vrai. du coup pour resoudre le problème, on ne test plus
        pageToAloccate=map->Find(pageToAloccate);
        ASSERT(pageToAloccate >= 0 && pageToAloccate < NumPhysPages);


  //    }while(map->Test(pageToAloccate));
      return pageToAloccate;
    }

    int FrameProvider::GetFirstFit(int indice){
      int ret = map->Find(indice);
      if(ret == -1)
        ret = map->Find();

      if(ret != -1)
      {
        Indice = ret;
      }

      return ret;
    }

#endif
