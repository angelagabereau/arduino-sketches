/*
  Clyde Tests
  Ambient Light part 2

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

int testNumber = 11;
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

  case 11:
    //setColor HSV
    Serial.println("11 setColor HSV blue");
    Clyde.ambientLight.setColor(HSV(0.67,1,255));
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 12:
    //setColor HSV
    Serial.println("12 setColor HSV white");
    Clyde.ambientLight.setColor(HSV(0,0,255));
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 13:
    //setColor HSV
    Serial.println("13 setColor HSV off");
    Clyde.ambientLight.setColor(HSV(0,0,0));
    Serial.println("isOn - it shouldn't be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 14:
    //turnOn
    Serial.println("14 turnOn");
    Clyde.ambientLight.turnOn();
    Serial.println("isOn - it should be");
    Serial.println(Clyde.ambientLight.isOn());
    break;

  case 15:
    //turnOff
    Serial.println("15 turnOff");
    Clyde.ambientLight.turnOff();
    Serial.println("isOn - it shouldn't be");
    Serial.println(Clyde.taskLight.isOn());
    break;

  case 16:
    //save
    Serial.println("16 save");
    //set color to magento 
    Clyde.ambientLight.setColor(HSV(0.83,1,255));
    //save
    Clyde.ambientLight.save();
    //turn off
    Clyde.ambientLight.turnOff();
    //turn on, should be magento
    Clyde.ambientLight.turnOn();
    Serial.println("isOn - it should be, and it should be magento");
    Serial.println(Clyde.ambientLight.isOn());
    break;


  case 17:
    //fadeTo RGB RED, from cyan
    //on first pass through this test we request a fade
    if(!stayInCurrentTest){
      //Set color to Cyan
      Clyde.ambientLight.setColor(RGB(0,255,255));
      delay(500);
      Serial.println("17 fadeTo red from cyan rgb / update");
      Clyde.ambientLight.fadeTo(RGB(255,0, 0), 0.01);
      stayInCurrentTest = true;
    }
    else{
      //on all subsequent passes, we perform the fade.
      Clyde.ambientLight.update();
    }

    //Once CLyde is On, we can move to the next test
    if(Clyde.ambientLight.redLight.brightness>=224){
      stayInCurrentTest = false;
    }

    break;

  case 18:
    //fadeTo RGB GREEN, from red
    //on first pass through this test we request a fade
    if(!stayInCurrentTest){
      //Set color to Cyan
      Clyde.ambientLight.setColor(RGB(255,0,0));
      delay(500);
      Serial.println("18 fadeTo green from red rgb / update");
      Clyde.ambientLight.fadeTo(RGB(0,255, 0), 0.01);
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

  case 19:
    //fadeTo RGB BLUE, from green
    //on first pass through this test we request a fade
    if(!stayInCurrentTest){
      //Set color to Cyan
      Clyde.ambientLight.setColor(RGB(0,255,0));
      delay(500);
      Serial.println("19 fadeTo blue from green rgb / update");
      Clyde.ambientLight.fadeTo(RGB(0,0, 255), 0.01);
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

    //HSV fadeTo

  case 20:
    //fadeTo HSV RED, from cyan
    //on first pass through this test we request a fade
    if(!stayInCurrentTest){
      //Set color to Cyan
      Clyde.ambientLight.setColor(HSV(0.5,1,255));
      delay(500);
      Serial.println("20 fadeTo red from cyan HSV / update");
      Clyde.ambientLight.fadeTo(HSV(0,1,255), 0.01);
      stayInCurrentTest = true;
    }
    else{
      //on all subsequent passes, we perform the fade.
      Clyde.ambientLight.update();
    }
    //Once CLyde is On, we can move to the next test
    if(Clyde.ambientLight.redLight.brightness>=224){
      stayInCurrentTest = false;
    }

    break;

  default:
    Serial.println("----------- Start Test Sequence -----------");
    testNumber = 10;
  }

  if(!stayInCurrentTest){
    delay(3000);
    testNumber++;
    Serial.println();
  }

}