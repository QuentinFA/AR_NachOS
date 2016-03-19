#ifdef CHANGED
#include "frameprovider.h"
#include "system.h"

    FrameProvider::FrameProvider(int nombrePages){
      map=new BitMap(nombrePages);
    }
    FrameProvider::~FrameProvider(){
      delete map;
    }
    int FrameProvider::GetEmptyFrame(){
        if(NumAvailFrame()==0)
        ASSERT("No frame available");

        int frameToAloccate;

        do{
          frameToAloccate=Random()%NumPhysPages;
        }while(map->Test(frameToAloccate));

        map->Mark(frameToAloccate);

      int  frameSize= frameToAloccate * PageSize;

        bzero(machine->mainMemory,frameSize);

        return frameToAloccate;
    }

    void FrameProvider::ReleaseFrame(int numPage){
        map->Clear(numPage);
    }
    int FrameProvider::NumAvailFrame(){
      return map->NumClear();
    }
#endif
