#include "Ancillary.h"

#define MAX_ARGS 20

bool commandReceived;
int argumentIndex;
String commandBuffer[MAX_ARGS];

Ancillary::Ancillary() { }

void Ancillary::begin(const char* deviceName, const char* manufacturer, const char* deviceType, 
  Stream &bus, UniqueID &uniqueId, void (*commandHandler)(String, String*)) {
  this->deviceName = deviceName;
  this->manufacturer = manufacturer;
  this->deviceType = deviceType;
  this->bus = &bus;
  this->uniqueId = &uniqueId;
  this->commandEvent = commandHandler;

  resetCommandBuffer();
}

void Ancillary::loop() {
  receiveIncomingData();
  if (commandReceived) {
    handleCommand();
    resetCommandBuffer();
  }

  sendHeartbeat();
}

void Ancillary::handleCommand() {
  bus->print("MSG\t");
  bus->print(uniqueId->getID(), HEX);
  bus->print("\t");
  for (int i = 0; i < MAX_ARGS; i++) {
    bus->print(commandBuffer[i]);
    bus->print("\t");  
  }
  bus->println("");

  String targetID = commandBuffer[1];

  if (targetID.equalsIgnoreCase(String(uniqueId->getID(), HEX))) {
    String command = commandBuffer[2];
    String commandId = commandBuffer[3];
    String args[MAX_ARGS];

    for (int i = 4; i < MAX_ARGS; i++) {
      args[i] = String(commandBuffer[i]);
    }
    
    commandEvent(command, args);

    bus->print("ACK\t");
    bus->print(uniqueId->getID(), HEX);
    bus->print("\t");
    bus->print(commandId);
    bus->println("");
  }
}

void Ancillary::resetCommandBuffer() {
  commandReceived = false;
  argumentIndex = 0;
  for (int i = 0; i < MAX_ARGS; i++) {
    commandBuffer[i] = "";
  }
}

void Ancillary::receiveIncomingData() {
  if (bus->available()) {
    while (bus->available()) {
      char inChar = (char)bus->read();
      if (inChar == '\t') {
        if (argumentIndex < MAX_ARGS) {
          argumentIndex += 1;
          commandBuffer[argumentIndex] = "";
        } else {
          // Drop further args to prevent buffer overflow.
          commandReceived = true;
        }
      } else if (inChar == '\n') {
        commandReceived = true;
      } else {
        commandBuffer[argumentIndex] += inChar;
      }
    }
  }
}

void Ancillary::sendHeartbeat() {
  bus->print("HEARTBEAT\t");
  bus->print(uniqueId->getID(), HEX);
  bus->print("\t");
  bus->print(deviceName);
  bus->print("\t");
  bus->print(manufacturer);
  bus->print("\t");
  bus->print(deviceType);
  bus->println("");
}
