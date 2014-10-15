#include <EEPROM.h>
#include <UniqueID.h>
#include <Ancillary.h>
#include "Device.h"

const char deviceName[] = "Example Device";
const char manufacturer[] = "Jon Wood";
const char deviceType[] = "Generic";

UniqueID uniqueID;
Ancillary ancillary;

void commandEvent(String command, String* args) {
  pinMode(13, HIGH);
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(13, LOW);
  
  Serial.begin(9600);
  ancillary.begin(deviceName, manufacturer, deviceType, Serial, uniqueID, commandEvent);
}

void loop() {
  ancillary.loop();
  delay(1000);
}
