/*
  Clyde Tests
  Afraid of the Dark

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
  Serial.begin(9600);
  delay(4000);

  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Test Afraid of the Dark");
  Serial.println();
}

void loop() {
  Clyde.update();
  delay(1000);
}

