#include "UniqueID.h"
#include <EEPROM.h>

UniqueID::UniqueID()
{
  // Depends on pin 0 being disconnected.
  randomSeed(analogRead(0));
}

long UniqueID::getID()
{
  if (EEPROM.read(0) == 255) {
    setID(random(254), random(254), random(254), random(254));
  }

  return readID();
}

long UniqueID::setID(int block1, int block2, int block3, int block4)
{
  EEPROM.write(0, block1);
  EEPROM.write(1, block2);
  EEPROM.write(2, block3);
  EEPROM.write(3, block4);
  
  return readID();
}

long UniqueID::readID()
{
  return EEPROM.read(0) * EEPROM.read(1) * EEPROM.read(2) * EEPROM.read(3);
}
