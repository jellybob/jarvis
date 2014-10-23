#include <EEPROM.h>
#include <UniqueID.h>
#include <Ancillary.h>
#include "RelayDevice.h"

const char deviceName[] = "Relay Controller Mk1";
const char manufacturer[] = "Jon Wood";
const char deviceType[] = "Relay";

UniqueID uniqueID;
Ancillary ancillary;

const int relayPin = 12;
bool relayState = LOW;

void commandEvent(String command, String* args) {
  if (command == "toggle") {
    if (relayState == HIGH) {
      relayState = LOW;
    } else {
      relayState = HIGH;
    }
  } else if (command == "on") {
     relayState = HIGH;     
  } else if (command == "off") {
     relayState = LOW; 
  }
  
  digitalWrite(relayPin, relayState);
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayState);
  
  Serial.begin(9600);
  ancillary.begin(deviceName, manufacturer, deviceType, Serial, uniqueID, commandEvent);
}

void loop() {
  ancillary.loop();
  delay(1000);
}
