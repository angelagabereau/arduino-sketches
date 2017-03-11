/*
  Clyde Tests
  Color Cycle

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

int testNumber = 1;
boolean stayInCurrentTest = false;//Some functionality requires multiple trips through the loop.  Use this flag to control that.
float hue = 0.0;


void setup() {
  Wire.begin();
  Clyde.begin();

  delay(2000);

  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Test Color Cycle");
  Serial.println();
}

void loop() {

  switch(testNumber){

  case 1:

    break;

  default:
    testNumber = 0;
  }

  if(!stayInCurrentTest){
    delay(2000);
    testNumber++;
    Serial.println();
  }

}
