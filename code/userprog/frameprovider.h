#ifdef CHANGED

#include "bitmap.h"
class FrameProvider{
public:
  int GetEmptyFrame();
  void ReleaseFrame(int numPage);
  int NumAvailFrame();
  int GetRandomFit();
  int GetFirstFit(int indice);
  FrameProvider(int nombrePages);
  ~FrameProvider();
  int numfois;
private:
  BitMap *map;
  int Indice;

};
#endif
