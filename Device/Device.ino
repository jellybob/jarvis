#include <EEPROM.h>
#include <UniqueID.h>
#include <Ancillary.h>
#include "Device.h"

const char deviceName[] = "Example Device";
const char manufacturer[] = "Jon Wood";
const char deviceType[] = "Generic";

UniqueID uniqueID;
Ancillary ancillary;

void setup() {
  Serial.begin(9600);
  ancillary.begin(deviceName, manufacturer, deviceType, Serial, uniqueID);
}

void loop() {
  ancillary.loop();
  delay(1000);
}
