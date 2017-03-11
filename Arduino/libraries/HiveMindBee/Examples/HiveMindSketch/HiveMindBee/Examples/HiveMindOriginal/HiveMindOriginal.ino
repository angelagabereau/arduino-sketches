#include <Wire.h>
#include <EEPROM.h>
#include <FabuleClydeDev.h>
#include <SerialCommand.h>
#include <SoftwareSerial.h>
#include <MPR121.h>

#define FIRMWARE_VERSION 1

void setup() {
  Wire.begin();
  Clyde.begin();
  Serial.println("Clyde is Ready!");
}

void loop() {

}
