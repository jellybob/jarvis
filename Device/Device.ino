#define DEVICE "Example Device"
#define MANUFACTURER "Jon Wood"
#define DEVICE_TYPE "Generic"

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
String data;
bool dataReceived;

void setup() {
  dataReceived = false;
  data.reserve(200);
  uniqueID = getUniqueID();
  
  DATA_BUS.begin(9600);
}

void loop() {
  if (DATA_BUS.available()) {
    serialEvent();  
  }
  
  if (dataReceived) {
    handleData();
    dataReceived = false;
    data = "";
  }
  
  sendHeartbeat();
  delay(1000);
}

void handleData() {
  digitalWrite(DEBUG_LED, HIGH);
  irsend.sendNEC(0x20DF10EF, 32);
  delay(500);
  digitalWrite(DEBUG_LED, LOW);
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

void serialEvent() {
  while (DATA_BUS.available()) {
    // get the new byte:
    char inChar = (char)DATA_BUS.read(); 
    // add it to the inputString:
    data += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      dataReceived = true;
    }
  }
}

long getUniqueID() {
  //if (FLASH.read(0) == 255) {
  //  for (int i = 0; i < 4; i++) {
  //    FLASH.write(i, random(255)); 
  //  }
  //}
  
  //long id = FLASH.read(0);
  //for (int i = 1; i < 4; i++) {
  //   id = id * FLASH.read(i);
  //}
  
  //return id;
  
  return 255;
}
