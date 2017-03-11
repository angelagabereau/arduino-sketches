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
#include "FabuleClydeDev.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "debugflags.h"


CClyde Clyde;

CClyde::CClyde() {
  Light taskLight = Light();
  ColorLight ambientLight = ColorLight();
  Chatter chatter = Chatter();
  SquishyEye squishyEye = SquishyEye();
}

void CClyde::begin() {

#ifdef CLYDE_DEBUG_WAIT
  delay(5000);
#endif

  //Initialize the task light.
  taskLight.init(TASK_PIN);

  //Initialize the ambient light.
  ambientLight.init(R_PIN, G_PIN, B_PIN);

  chatter.init();

  squishyEye.init();

}

/**
 * TASK LIGHT
 */
/*
void CClyde::updateWhiteLight() {
  taskLight.update();
}
void CClyde::showWhiteLight() {
  taskLight.show();
}
void CClyde::fadeWhite(uint8_t b, float spd) {
  taskLight.fade(b,spd);
}
void CClyde::setWhite(uint8_t b) {
  taskLight.setBright(b);
}
*/

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


/**
 * SQUISHY EYE BUTTON
 */
/*
void CClyde::updateEye() {
  squishyEye.update();
}

void CClyde::calibrateEye(uint16_t irValue) {  
  Clyde.squishyEye.calibrate(irValue)
}

bool CClyde::wasEyePressed(uint16_t irValue) {
  squishyEye.wasPressed(irValue);

}

*/