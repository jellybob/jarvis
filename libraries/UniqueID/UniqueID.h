#include <Arduino.h>
#ifndef UniqueID_h
#define UniqueID_h

class UniqueID
{
public:
  UniqueID();
  long getID();
  long setID(int block1, int block2, int block3, int block4);
protected:
  long readID();
};
#endif
