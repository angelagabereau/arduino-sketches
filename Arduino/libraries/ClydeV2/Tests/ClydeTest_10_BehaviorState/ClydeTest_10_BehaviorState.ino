/*
  Clyde Tests
  Behavior State

  This example code is in the public domain.

  modified 16 Feb 2015
  by Angela Gabereau
 */

#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <MPR121.h>
#include <ClydeV2.h>

//Test Behavior State

#define FIRMWARE_VERSION 2

void setup() {
  
  Wire.begin();
  Clyde.begin();

  Serial.begin(9600); // start serial for debugging output
  delay(2000);
  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Behavior State");
  Serial.println();
  
  // Clyde.ambientLight.setColor(RGB(255,255,255));
  // Clyde.ambientLight.fadeTo(RGB(0,255, 0), 5);

  // Clyde.ambientLight.startColorSelect(1);
  // Clyde.ambientLight.startBlink(RGB(255,0,0), 1000, 1000, 0);
  
  // Clyde.startBlinkBehavior(RGB(255,0,255),1000, 1000,0);
  // Clyde.endBehavior();

  // Clyde.startSelectBehavior();
  // Clyde.endBehavior();

  // Clyde.startLaughBehavior();
  // Clyde.endBehavior();

  Clyde.startSunsetBehavior();
  
  Serial.println("end of setup");
}

void loop() {
  
  Clyde.update();
  
}
