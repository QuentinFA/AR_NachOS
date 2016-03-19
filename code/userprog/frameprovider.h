#ifdef CHANGED

#include "bitmap.h"
class FrameProvider{
public:
  int GetEmptyFrame();
  void ReleaseFrame(int numPage);
  int NumAvailFrame();
  FrameProvider(int nombrePages);
  ~FrameProvider();
private:
  BitMap *map;
};
#endif
