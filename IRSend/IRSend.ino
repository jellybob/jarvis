#include <Arduino.h>
#include <IRremote2.h>

#define LED PIN_LED_RXL
#define BUTTON BUTTON_A

int i = 0;
long codes[4] = { 0xF720DF, 0xF7A05F, 0xF7609F, 0xF7E01F };

IRsend irsend;

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    digitalWrite(LED, HIGH);
    irsend.sendNEC(codes[i], 32);
    i += 1;
    if (i > 3) { i = 0; }
    delay(50);
  } else {
    digitalWrite(LED, LOW);
  }
}
