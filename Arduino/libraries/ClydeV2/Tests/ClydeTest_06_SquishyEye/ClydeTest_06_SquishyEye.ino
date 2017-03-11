/*
  Clyde Tests
  Squishy Eye

  This example code is in the public domain.

  modified 16 Feb 2015
  by Angela Gabereau
 */

#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <MPR121.h>
#include <ClydeV2.h>

#define FIRMWARE_VERSION 2

void setup() {
  Wire.begin();
  Clyde.begin();

  delay(2000);

  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Test SquishyEye");
  Serial.println();
}

void loop() {
    Clyde.squishyEye.update(&Clyde);
}