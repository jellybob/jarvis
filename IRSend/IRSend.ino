#include <IRremote2.h>

#if defined(__SAM3X8E__) || defined(__SAM3X8H)
  // This is an Arudino Due, probably my Tilda MKe
  #define LED 72
  #define DEBUG SerialUSB
#else
  // Its something else
  #define LED 13
  #define DEBUG Serial
#endif

IRsend irsend;

void setup() {
//  while(!SerialUSB);
  DEBUG.begin(9600);
  DEBUG
}

void loop() {
  DEBUG.print(".");
  digitalWrite(LED, HIGH);
  irsend.sendNEC(0x20DF10EF, 32);
  delay(1000);
  
  digitalWrite(LED, LOW);
  delay(1000);
}
