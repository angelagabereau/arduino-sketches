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

#include "ColorLight.h"
#include "colortypes.h"
#include "debugflags.h"

const float ColorLight::SCALE_CONSTRAINT = 225.0f / 255.0f;

/**
 * Constructor
 */
ColorLight::ColorLight() {

  this->color = RGB(0, 0, 0);
  this->savedColor = RGB(255, 255, 255); //White

  setColor(RGB(0,0,0));

  this->redLight = Light();
  this->greenLight = Light();
  this->blueLight = Light();

}
/**
 * Initaliaze the Color Light
 */
bool ColorLight::init(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin ) {

  //Initialize the Lights with the pins numbers.
  redLight.init(r_pin);
  greenLight.init(g_pin);
  blueLight.init(b_pin);

  //set default lights off
  this->setColor(RGB(0,0,0));

  #ifdef CLYDE_DEBUG
  Serial.println("Clyde: Color Light initialized.");
  #endif
  
  return true;
}

/**
 * Set Color, in RGB, wipes out fade stepping.
 */
void ColorLight::setColor(const RGB &c) {
  //Set color.
  color = c;
  targetColor = c;

  #ifdef CLYDE_DEBUG
  Serial.println("Clyde ColorLight::setColor(const RGB &c)");
  Serial.println(color.r);
  Serial.println(color.g);
  Serial.println(color.b);
  #endif


  //Pass the new color on to the Lights
  redLight.setBrightness(color.r * ColorLight::SCALE_CONSTRAINT);
  blueLight.setBrightness(color.b * ColorLight::SCALE_CONSTRAINT);
  greenLight.setBrightness(color.g * ColorLight::SCALE_CONSTRAINT);

  show();
}

/**
 * Set Color, in HSV, wipes out fade stepping.
 */
void ColorLight::setColor(const HSV &hsv) {

  #ifdef CLYDE_DEBUG
  Serial.println("Clyde ColorLight::setColor(const HSV &hsv)");
  Serial.println(hsv.h);
  Serial.println(hsv.s);
  Serial.println(hsv.v);
  #endif

  setColor(colorConverter.HSVtoRGB(hsv));
}

/**
 * Show, set the pin to current color.
 * previously CClyde::showAmbientLight()
 */
void ColorLight::show() {

  //output new color
  redLight.show();
  blueLight.show();
  greenLight.show();
}


/**
 * update
 * previously CClyde::updateAmbientLight()
 */
void ColorLight::update() {
  //update ambient cycle
 // if (m_cycle.isOn())
  //  updateCycle();

  //fade each rgb value
  redLight.update();
  greenLight.update();
  blueLight.update();

}


/**
 * fadeTo
 * initiate a fade to specified color.  requires call to update in loop
 */
void ColorLight::fadeTo(const RGB &c, float spd) {

  #ifdef CLYDE_DEBUG_LIGHT
  Serial.print("Color Light fadeTo");
  #endif

  targetColor = c;
  redLight.fadeTo(targetColor.r, spd);
  greenLight.fadeTo(targetColor.g, spd);
  blueLight.fadeTo(targetColor.b, spd);
}

/**
 * fade -  this is fade request...
 * previously ...
 */
void ColorLight::fadeTo(const HSV &c, float spd) {
  fadeTo(colorConverter.HSVtoRGB(c), spd);
}

/**
 * Turn the light on.
 */
void ColorLight::turnOn(){
    setColor(savedColor);
}

/**
 * Turn the light off.
 */
void ColorLight::turnOff(){
  setColor(RGB(0,0,0));
}

/**
* Check if the ambient light is on.
*/
bool ColorLight::isOn(){
  return targetColor.r > 0 || targetColor.g > 0 || targetColor.b > 0;
}

/** Save the current color. */
void ColorLight::save() { if (color.r + color.g + color.b > 10) savedColor = RGB(color.r, color.g, color.b); }


/*
void ColorLight::blink(const RGB& rgb, uint32_t onDuration, uint32_t offDuration, uint8_t numBlinks) {
  //calculate number of steps needed in the cycle
  uint8_t steps = numBlinks*2 + 1;
  
  //if numBlinks was zero (infinite loop), make space for on/off
  if (steps == 1) steps = 2;
  
  //check number of step limit
  if (steps > CAmbientCycle::MAX_CYCLE_LENGTH)
    return;
  
  //set blinks color
  RGB colors[steps];
  uint32_t intervals[steps];
  
  for(int i = 0; i < steps; i++) {
    if (i%2==1) {
      colors[i] = rgb;
      intervals[i] = offDuration;
    }
    else {
      intervals[i] = onDuration;
    }
  }
  
  setCycle(BLINK, steps, &colors[0], &intervals[0], numBlinks==0?LOOP:NO_LOOP);
}
*/

float ColorLight::getHue() {
  //Get Current Hue
  HSV hsv = colorConverter.RGBtoHSV(this->color);
  return hsv.h; //This is the hue
}
