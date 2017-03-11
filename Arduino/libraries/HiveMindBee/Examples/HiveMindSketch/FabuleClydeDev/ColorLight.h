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

#ifndef __CLYDE_COLORLIGHT_H
#define __CLYDE_COLORLIGHT_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "colortypes.h"
#include "Light.h"

/**
 * The Color RGB Light
 * A composite of three Light objects, one for red, green and blue.
 * and a memory of its current and target color.
 * Ability to fade and blink.
 */
class ColorLight {

  static const float SCALE_CONSTRAINT;  /**< Intensity constraint because of conflict with eye's IR sensor. */

public:
  RGBf color;                           /**< Current color. */
  RGB targetColor;                      /**< Target color, used for fading. */
  RGB savedColor;                       /**< Saved ambient color to go back to. */
  float hue;
  ColorConverter colorConverter;

public:  
  /** Constructor. */
  ColorLight();

  /**
   * Initialize the module with specified analog and digital pins.
   */  
  bool init(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin );

  /**
   * Update the ambient light.
   */  
  void update();

  /**
   * show
   */  
  void show();

  /**
   * Turn the light on.
   */
  void turnOn();

  /**
   * Turn the light off.
   */
  void turnOff();

  /**
   * Check if the light is on.
   */
  bool isOn();

  /** 
   * Save the current color. 
   */
  void save();

  /**
   * Fade the light to a given color.
   */
  void fadeTo(const RGB &c, float spd);

  void fadeTo(const HSV &c, float spd);

  /**
   * Set the color of the ColorLight
   */
  void setColor(const RGB &c);

  void setColor(const HSV &c);

  /**
   * Blink the light
   */
//  void blink();

  float getHue();


  Light redLight;
  Light greenLight;
  Light blueLight;
  
};


//extern ColorLight ColorLight;

#endif
