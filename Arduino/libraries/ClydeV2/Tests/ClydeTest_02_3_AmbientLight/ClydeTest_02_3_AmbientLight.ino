/*
  Clyde Tests
  Ambient Light part 3

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

int testNumber = 21;
boolean stayInCurrentTest = false;//Some functionality requires multiple trips through the loop.  Use this flag to control that.
float hue = 0.0;

void setup() {
  Wire.begin();
  Clyde.begin();

  delay(2000);

  Serial.begin(9600);
  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Test AmbientLight");
  Serial.println();
}

void loop() {

  switch(testNumber){
  
  case 21:
    //fadeTo HSV GREEN, from red
    //on first pass through this test we request a fade
    if(!stayInCurrentTest){
      //Set color to red
      Clyde.ambientLight.setColor(HSV(0,1,255));
      delay(500);
      Serial.println("21 fadeTo green from red HSV / update");
      Clyde.ambientLight.fadeTo(HSV(0.33,1,255), 0.01);
      stayInCurrentTest = true;
    }
    else{
      //on all subsequent passes, we perform the fade.
      Clyde.ambientLight.update();
    }
    //Once CLyde is On, we can move to the next test
    if(Clyde.ambientLight.greenLight.brightness>=224){
      stayInCurrentTest = false;
    }

    break;

  case 22:
    //fadeTo HSV BLUE, from green
    //on first pass through this test we request a fade
    if(!stayInCurrentTest){
      //Set color to green
      Clyde.ambientLight.setColor(HSV(0.33,1,255));
      delay(500);
      Serial.println("22 fadeTo blue from green hsv / update");
      Clyde.ambientLight.fadeTo(HSV(0.67,1,255), 0.01);
      stayInCurrentTest = true;
    }
    else{
      //on all subsequent passes, we perform the fade.
      Clyde.ambientLight.update();
    }
    //Once CLyde is On, we can move to the next test
    if(Clyde.ambientLight.blueLight.brightness>=224){
      stayInCurrentTest = false;
    }

    break;

  case 23:   
    //getHue, red
    Clyde.ambientLight.setColor(HSV(0,1,255));
    Serial.println("23 get hue, should be 0");
    Serial.println(Clyde.ambientLight.getHue());
    break;

  case 24:   
    //getHue, red
    Clyde.ambientLight.setColor(HSV(0.33,1,255));
    Serial.println("24 get hue, should be 0.33");
    Serial.println(Clyde.ambientLight.getHue());
    break;

  case 25:   
    //getHue, red
    Clyde.ambientLight.setColor(HSV(0.67,1,255));
    Serial.println("25 get hue, should be 0.67");
    Serial.println(Clyde.ambientLight.getHue());
    break;

  case 26:   
 
    //cycle through hue, red to blue
    if(!stayInCurrentTest){
      //Set color to red
      Clyde.ambientLight.setColor(HSV(hue,1,255));
      delay(500);
      Serial.println("Cycle through colors with hue");
      stayInCurrentTest = true;
    }
    else{
      Serial.println(hue);
      Clyde.ambientLight.setColor(HSV(hue,1,255));
      hue = hue + 0.001;
      delay(50);
    }
    //Once CLyde is On, we can move to the next test
    if(hue>=1){
      stayInCurrentTest = false;
      hue = 0.0;
    }
    break;

  default:
    Serial.println("----------- Start Test Sequence -----------");
    testNumber = 20;
  }

  if(!stayInCurrentTest){
    delay(3000);
    testNumber++;
    Serial.println();
  }

}