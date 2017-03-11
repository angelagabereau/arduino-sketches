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

#include "Light.h"
#include "debugflags.h"

Light::Light() {

  brightness = 255;       /**< Current brightness. */
  targetBrightness = 255; /**< Target brightness, used for fading. */
  fadeSpeed = 0.0;   
}


/**
* Initialize the module with specified analog and digital pins.
*/
bool Light::init(uint8_t pin) {
 
  //setup white light pins
  this->pin = pin;	
  pinMode(this->pin, OUTPUT);
  analogWrite(this->pin, brightness);
  
  #ifdef CLYDE_DEBUG_LIGHT
  Serial.println("Clyde: Light initialized.");
  #endif

  //init white light
  //we use 254 brightness as a hack to remove the flicker on first fade-in,
  //but this adds a flash on startup instead, which is kinda neat
  //TODO look for a better solution
  setBrightness(255);
  
  return true;
}

  /**
   * Turn the light on.
   */
  void Light::turnOn(){
    setBrightness(0);
  }

  /**
   * Turn the light off.
   */
  void Light::turnOff(){
    setBrightness(255);
  }

  /**
   * Blink the light
   */
  //void blink();

  /**
   * Check if the light is on.
   */
  bool Light::isOn(){ 
    return targetBrightness < 255; 
  }


/**
 * Update
 * previously updateWhiteLight()
 */
void Light::update() {
  //only fade if we haven't reached the desired level
  if (this->targetBrightness == this->brightness)
    return;

  //Calculate the next brightness value for fade.
  fadeStep();

  //output new level
  show();
}

/**
 * Show
 * set pin brightness, maintains fade stepping increments
 * previously showWhiteLight()
 */
void Light::show() {
  analogWrite(this->pin, this->brightness);
}

/**
  * Request a fade to specified brightness.
  * spd controls how many steps/interations it will take
  * previously fadeWhite(uint8_t b, float spd)
  */
void Light::fadeTo(uint8_t b, float spd) {
  this->targetBrightness = b;

  this->fadeSpeed = (this->targetBrightness - this->brightness) / 255.0 * spd;

  #ifdef CLYDE_DEBUG_LIGHT
  Serial.print("Light fadeTo:  this->fadeSpeed");
  Serial.println(this->fadeSpeed);
  #endif

  if (this->fadeSpeed < 0) this->fadeSpeed *= -1;
}

void Light::fadeStep(){

  //get the difference to the target, and the fade direction
  float diff = this->targetBrightness - this->brightness;
  int8_t dir = diff < 0 ? -1 : 1;

  //fade and limit to the defined max fade speed
  this->brightness += diff*dir < this->fadeSpeed ? diff : this->fadeSpeed*dir;

      #ifdef CLYDE_DEBUG_LIGHT
      Serial.print("Light fadeStep diff:");
      Serial.print(diff);
      Serial.print(" dir: ");
      Serial.print(dir);
      Serial.print(" brightness: ");
      Serial.println(this->brightness);
      #endif
}

/**
 * Set the brightness of the light.
 * wipes out any fade stepping.
 */
void Light::setBrightness(uint8_t b) {
  this->brightness = this->targetBrightness = b;
  show();
}