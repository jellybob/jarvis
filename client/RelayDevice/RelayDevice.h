#ifndef DEVICE_H
#define DEVICE_H

void handleCommand();
void resetCommandBuffer();
void sendHeartbeat();
void blinkLed(int duration);
void serialEvent();
long getUniqueID();

#endif
