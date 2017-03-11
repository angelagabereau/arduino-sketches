/*
  Copyright (c) 2013-2014, Fabule Fabrications Inc, All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, version 3.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library.
*/
#include "HiveMindBee.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "debugflags.h"

HiveMindBee::HiveMindBee() {

  this->colorLight = ColorLight();

}

void HiveMindBee::begin(int8_t id) {

#ifdef BEE_DEBUG_WAIT
  delay(5000);
#endif

  this->id = id;
  //Initialize the color light.
  colorLight.init(R_PIN, G_PIN, B_PIN);

  colorLight.setColor(RGB(255, 0, 255));

}

void HiveMindBee::update() {
  colorLight.update();
}


void HiveMindBee::lightOn(int8_t r, int8_t g, int8_t b){

  #ifdef BEE_DEBUG
    Serial.print("lightOn: ");
    Serial.print(" id: ");
    Serial.print(this->id);
    Serial.print(" r: ");
    Serial.print(r);
    Serial.print(" g: ");
    Serial.print(g);
    Serial.print(" b: ");
    Serial.print(b);
    Serial.println();
  #endif

  colorLight.setColor(RGB(r, g, b));
}

void HiveMindBee::lightOff(){

  #ifdef BEE_DEBUG
    Serial.print("lightOff: ");
    Serial.print("id: ");
    Serial.print(this->id);
    Serial.println();
  #endif

  colorLight.setColor(RGB(0, 0, 0));
}

void HiveMindBee::shiftColor(int8_t r, int8_t g, int8_t b){

  #ifdef BEE_DEBUG
    Serial.print("shiftColor: ");
    Serial.print("id: ");
    Serial.print(this->id);
    Serial.print("r: ");
    Serial.print(r);
    Serial.print("g: ");
    Serial.print(g);
    Serial.print("b: ");
    Serial.print(b);
    Serial.println();
  #endif

  colorLight.fadeTo(RGB(r, g, b), 0.5f);
}

void HiveMindBee::startCycle(){

  #ifdef BEE_DEBUG
    Serial.print("startCycle: ");
    Serial.print("id: ");
    Serial.print(this->id);
    Serial.println();
  #endif

}

void HiveMindBee::stopCycle(){

  #ifdef BEE_DEBUG
    Serial.print("stopCycle: ");
    Serial.print("id: ");
    Serial.print(this->id);
    Serial.println();
  #endif

}

void HiveMindBee::pulse(int8_t r, int8_t g, int8_t b){

  #ifdef BEE_DEBUG
    Serial.print("pulse: ");
    Serial.print("id: ");
    Serial.print(this->id);
    Serial.print("r: ");
    Serial.print(r);
    Serial.print("g: ");
    Serial.print(g);
    Serial.print("b: ");
    Serial.print(b);
    Serial.println();
  #endif

}

void HiveMindBee::displayText(String text){
  #ifdef BEE_DEBUG
    Serial.print("displayText: ");
    Serial.print("id: ");
    Serial.print(this->id);
    Serial.print("text: ");
    Serial.print(text);
    Serial.println();
  #endif

}

void HiveMindBee::clearDisplay(){
  #ifdef BEE_DEBUG
    Serial.print("clearDisplay: ");
    Serial.print("id: ");
    Serial.print(this->id);
    Serial.println();
  #endif

}

/**
 * EYE LIGHT
 */
/*
void CClyde::updateAmbientLight() {
  eyeLight.update()
}

void CClyde::updateAmbientLight(float *value, uint8_t target, float speed) {
  eyeLight.update()
}

void CClyde::showAmbientLight() {
  //output new color
  eyeLight.show() 
}

void CClyde::setAmbient(const RGB &c) {
  eyeLight.setColor(c);
}

void CClyde::fadeAmbient(const RGB &c, float spd) {
  eyeLight.fade(c,spd);
}

void CClyde::blink(const RGB& rgb, uint32_t onDuration, uint32_t offDuration, uint8_t numBlinks) {
  eyeLight.blink(rgb, onDuration, offDuration, numBlinks);
}
*/