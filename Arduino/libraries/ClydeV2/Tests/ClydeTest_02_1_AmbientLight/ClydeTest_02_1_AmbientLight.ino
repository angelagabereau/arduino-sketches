/*
  Clyde Tests
  Ambient Light part 1

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

  Serial.begin(9600);
  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Test AmbientLight");
  Serial.println();
}

void loop() {

  switch(testNumber){
    //RGB COLOR SPACE
  case 1:
    //setColor RGB
    Serial.println("1 setColor RGB red");
    Clyde.ambientLight.setColor(RGB(255,0,0));

    break;

  case 2:
    //setColor RGB
    Serial.println("2 setColor RGB green");
    Clyde.ambientLight.setColor(RGB(0,255,0));
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 3:
    //setColor RGB
    Serial.println("3 setColor RGB blue");
    Clyde.ambientLight.setColor(RGB(0,0,255));
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 4:
    //setColor RGB
    Serial.println("4 setColor RGB white");
    Clyde.ambientLight.setColor(RGB(255,255,255));
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 5:
    //setColor RGB
    Serial.println("5 setColor RGB off");
    Clyde.ambientLight.setColor(RGB(0,0,0));
    Serial.println("isOn - it shouldn't be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 6:
    //turnOn
    Serial.println("6 turnOn");
    Clyde.ambientLight.turnOn();
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 7:
    //turnOff
    Serial.println("7 turnOff");
    Clyde.ambientLight.turnOff();
    Serial.println("isOn - it shouldn't be");
    Serial.println(Clyde.taskLight.isOn());
    break;

  case 8:
    //save
    Serial.println("8 save");
    //set color to magento 
    Clyde.ambientLight.setColor(RGB(255,0,255));
    //save
    Clyde.ambientLight.save();
    //turn off
    Clyde.ambientLight.turnOff();
    //turn on, should be magento
    Clyde.ambientLight.turnOn();
    Serial.println("isOn - it should be, and it should be magento");
    Serial.println(Clyde.ambientLight.isOn());
    break;


    //HSV COLOR SPACE
  case 9:
    //setColor HSV
    Serial.println("9 setColor HSV red");
    Clyde.ambientLight.setColor(HSV(0,1,255));

    break;

  case 10:
    //setColor HSV
    Serial.println("10 setColor HSV green");
    Clyde.ambientLight.setColor(HSV(0.33,1,255));
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  default:
    Serial.println("----------- Start Test Sequence -----------");
    testNumber = 0;
  }

  if(!stayInCurrentTest){
    delay(3000);
    testNumber++;
    Serial.println();
  }

}