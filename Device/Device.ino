#define DEVICE "Example Device"
#define MANUFACTURER "Jon Wood"
#define DEVICE_TYPE "Generic"

#define MAX_ARGS 20

#if defined(__SAM3X8E__) || defined(__SAM3X8H)
  #include <DueFlashStorage.h>
  DueFlashStorage dueFlashStorage;
  #define FLASH dueFlashStorage
  
  #define DATA_BUS SerialUSB
  #define DEBUG_LED 72
#else
  #include <EEPROM.h>
  #define FLASH EEPROM
  
  #define DATA_BUS Serial
  #define DEBUG_LED 13
#endif

#include <IRremote2.h>
IRsend irsend;

long uniqueID;
String commandBuffer[MAX_ARGS];
bool commandReceived;

void setup() {
  resetCommandBuffer();
  uniqueID = getUniqueID();
  
  DATA_BUS.begin(9600);
}

void loop() {
  if (DATA_BUS.available()) {
    serialEvent();
  }
  
  if (commandReceived) {
    handleCommand();
    resetCommandBuffer();
  }
  
  sendHeartbeat();
  delay(1000);
}

void handleCommand() {  
  DATA_BUS.print("MSG\t");
  DATA_BUS.print(uniqueID, HEX);
  DATA_BUS.print("\t");
  for (int i = 0; i < MAX_ARGS; i++) {
    DATA_BUS.print(commandBuffer[i]);
    DATA_BUS.print("\t");  
  }
  DATA_BUS.print("\n");
  
  String targetID = commandBuffer[1];
  
  if (targetID.equalsIgnoreCase(String(uniqueID, HEX))) {
    DATA_BUS.print("RESPONSE\t");
    DATA_BUS.print(uniqueID, HEX);
    DATA_BUS.print("\n");
    
    //digitalWrite(DEBUG_LED, HIGH);
    //irsend.sendNEC(0x20DF10EF, 32);
    //delay(500);
    //digitalWrite(DEBUG_LED, LOW);
  }
}

void resetCommandBuffer() {
  commandReceived = false;
  for (int i = 0; i < MAX_ARGS; i++) {
    commandBuffer[i] = "\0";
  }
}

void sendHeartbeat() {
  DATA_BUS.print("HEARTBEAT\t");
  DATA_BUS.print(uniqueID, HEX);
  DATA_BUS.print("\t");
  DATA_BUS.print(DEVICE);
  DATA_BUS.print("\t");
  DATA_BUS.print(MANUFACTURER);
  DATA_BUS.print("\t");
  DATA_BUS.print(DEVICE_TYPE);
  DATA_BUS.print("\n");
}

void blinkLed(int duration) {
  digitalWrite(DEBUG_LED, HIGH);
  delay(duration);
  digitalWrite(DEBUG_LED, LOW);
  delay(duration);
}

void serialEvent() {
  int i = 0;
  
  while (DATA_BUS.available()) {
    // get the new byte:
    char inChar = (char)DATA_BUS.read();
    
    if (inChar == '\t') {
      if (i < MAX_ARGS) {
        i += 1;
        commandBuffer[i] = "";
      } else {
        // Drop any further arguments to prevent overflowing.
        commandReceived = true;
      }
    } else if (inChar == '\n') {
      // Done
      commandReceived = true; 
    } else {
      // Push onto the current argument
      commandBuffer[i] += inChar;
    }
  }
}

long getUniqueID() {
  if (FLASH.read(0) == 255) {
    for (int i = 0; i < 4; i++) {
     FLASH.write(i, random(255)); 
    }
  }
  
  long id = FLASH.read(0);
  for (int i = 1; i < 4; i++) {
     id = id * FLASH.read(i);
  }
  
  return id;
}
