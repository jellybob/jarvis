#ifndef ANCILLARY_H
#define ANCILLARY_H

#include <Arduino.h>
#include <UniqueID.h>

class Ancillary
{
public:
  Ancillary();
  void begin(const char* deviceName, const char* manufacturer, const char* deviceType, Stream &bus, UniqueID &uniqueId, void (commandHandler)(String, String*));
  void loop();
protected:
  void receiveIncomingData();
  void handleCommand();
  void resetCommandBuffer();
  void sendHeartbeat();
  
  const char* deviceName;
  const char* manufacturer;
  const char* deviceType;
  Stream* bus;
  UniqueID* uniqueId;
  void (*commandEvent)(String, String*);
};

#endif
