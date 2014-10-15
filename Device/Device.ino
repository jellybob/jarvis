#include <EEPROM.h>
#include <UniqueID.h>
#include <Ancillary.h>
#include "Device.h"

const char deviceName[] = "Example Device";
const char manufacturer[] = "Jon Wood";
const char deviceType[] = "Generic";

UniqueID uniqueID;
Ancillary ancillary;
bool lightState = LOW;

void commandEvent(String command, String* args) {
  if (command == "toggle") {
    if (lightState == HIGH) {
      lightState = LOW;
    } else {
      lightState = HIGH;  
    }
    
    pinMode(13, lightState);
  }
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(13, LOW);
  
  Serial.begin(9600);
  ancillary.begin(deviceName, manufacturer, deviceType, Serial, uniqueID, commandEvent);
}

void loop() {
  ancillary.loop();
  delay(200);
}
