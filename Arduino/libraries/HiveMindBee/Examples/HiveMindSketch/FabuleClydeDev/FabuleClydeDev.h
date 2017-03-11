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
#ifndef __CLYDE_H
#define __CLYDE_H

#include <stdint.h>
#include <SoftwareSerial.h>
#include "clydepins.h"
#include "colortypes.h"


#include "Chatter.h"
#include "Light.h"
#include "ColorLight.h"
#include "SquishyEye.h"
#include "ClydeEEPROM.h"

/**
 * Main Clyde class that provides the interface to the device.
 */
class CClyde {
public:

  Light taskLight; //was CWhiteLight m_white;
  ColorLight ambientLight; //was CAmbientLight m_ambient;
  Chatter chatter; //was CMouth
  SquishyEye squishyEye; //was CEye

private:



public:
  /** Contructor. */
  CClyde();
  
  /** Initialize Clyde. */
  void begin();
  
  /** Update the ambient light. */
 // void updateAmbientLight(); is now taskLight.update();

  /** Update the white light. */
 // void updateWhiteLight(); is now eyeLight.update();

  

  /**
   * Get the ambient light object.
   */
 // CAmbientLight* ambient() { return &m_ambient; } is now this->eyeLight
  
  /**
   * Set ambient color.
   */
 // void setAmbient(const RGB &c); is now eyeLight->setColor();
  
  /**
   * Fade the ambient color to a given color.
   */
  //void fadeAmbient(const RGB &c, float spd); is now eyeLight->fade(const RGB &c, float spd);
    

  
  
  /**
   * Make the ambient light blink.
   */
  //void blink(const RGB& rgb, uint32_t onDuration, uint32_t offDuration, uint8_t numBlinks);
  


private:

  
  /** Update a color channel of the ambient light. */
 // void updateAmbientLight(float *value, uint8_t target, float speed); is now update();
  
  /** Show the current ambient light color. */
 // void showAmbientLight(); is now eyeLight.show();
    
  /** Show the current white light brightness. */
 // void showWhiteLight(); is now taskLight.show();
  
};

extern CClyde Clyde; 

#endif
