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
#ifndef __BEE_H
#define __BEE_H

#include <stdint.h>
#include <SoftwareSerial.h>
#include "beepins.h"
#include "colortypes.h"

#include "ColorLight.h"


/**
 * Main HiveMind Bee class that provides the interface to the device.
 */
class HiveMindBee {
public:
  uint8_t id;               /**< Bee ID Number */
  ColorLight colorLight;

private:



public:
  /** Contructor. */
  HiveMindBee();
  
  /** Initialize HiveMindBee. */
  void begin(int8_t id);
  void update();
  void lightOn(int8_t r, int8_t g, int8_t b);
  void lightOff();
  void shiftColor(int8_t r, int8_t g, int8_t b);
  void startCycle();
  void stopCycle();
  void pulse(int8_t r, int8_t g, int8_t b);
  void displayString(String text);
  void clearDisplay();

private:

  
};

#endif
