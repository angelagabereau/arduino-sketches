/*
  Clyde Tests
  Color Types

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

RGB color;
HSV hsv;
RGB rgb;
ColorConverter converter;

void setup() {
  Wire.begin();
  Clyde.begin();

  delay(2000);

  Serial.begin(9600);
  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Test Color Types");

  // color = RGB(255,0,255);
  // hsi = converter.RGBtoHSI(color);
  // rgb = converter.HSItoRGB(hsi);
}

void loop() {

  Serial.println("----------------------------------------");
  delay(2000);

  Serial.println("Clyde Color Conversion!");
  Serial.println("----------- Start Test Sequence -----------");

  //white
  Serial.println("white");
  color = RGB(255,255,255);
  testColorCoversion(color);

  //black
  Serial.println("black");
  color = RGB(0,0,0);
  testColorCoversion(color);

  //red
  Serial.println("red");
  color = RGB(255,0,0);
  testColorCoversion(color);

  //green
  Serial.println("green");
  color = RGB(0,255,0);
  testColorCoversion(color);

  //blue
  Serial.println("blue");
  color = RGB(0,0,255);
  testColorCoversion(color);

  //yellow
  Serial.println("yellow");
  color = RGB(255,255,0);
  testColorCoversion(color);

  //magenta
  Serial.println("magenta");
  color = RGB(255,0,255);
  testColorCoversion(color);

  //cyan
  Serial.println("cyan");
  color = RGB(0,255,255);
  testColorCoversion(color);

  delay(4000);
}

void testColorCoversion(RGB color){

  //Set as RGB
  Clyde.ambientLight.setColor(color);
  delay(1000);

  //Set as HSV
  hsv = converter.RGBtoHSV(color);
  Clyde.ambientLight.setColor(hsv);
  delay(1000);

  //Set as RGB
  rgb = converter.HSVtoRGB(hsv);
  Clyde.ambientLight.setColor(rgb);
  delay(1000);
  
  //Off
  Clyde.ambientLight.setColor(RGB(0,0,0));
  delay(1000);
}
