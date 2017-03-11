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
#ifndef __CLYDE_COLORCYCLE_H
#define __CLYDE_COLORCYCLE_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "colortypes.h"
#include "ColorLight.h"

/**
 * Enum types of ambient color cycles.
 */
enum ECycleType {
  OFF,
  BLINK,
  SUNSET,
  SELECT,
  LAUGH,
  UNKNOWN
};

/**
 * Enum types for cycle loop options.
 */
enum ECycleLoop {
  LOOP,
  LOOP_ONCE,
  NO_LOOP
};

/**
 * The Color Cycle
 */
class ColorCycle {

  ColorLight* ambientLight;	
  static const uint8_t MAX_CYCLE_LENGTH = 12; /**< Maximum cycle steps. */
  ECycleType type;                            /**< Type of the current cycle. */
  uint8_t numSteps;                           /**< Number of steps. */
  uint8_t step;                               /**< Current step. */
  uint32_t stepStart;                         /**< Time when the current step started in millis. */
  uint32_t stepEnd;                           /**< Time when the current step end in millis. */
  RGB stepColor;                              /**< Start color of the current step. */
  RGB colors[MAX_CYCLE_LENGTH];               /**< Colors of cycle. */
  uint32_t intervals[MAX_CYCLE_LENGTH];       /**< Intervals in millis. */
  ECycleLoop loop;                            /**< Looping parameter. */

public:  
  /** Constructor. */
  ColorCycle();
  
  /**
   * Initialize 
   */  
  bool init(ColorLight* ambientLight);

  /**
   * Check if there is an ongoing cycle.
   */
  bool isOn();
    
  /**
   * Check if the current cycle matches a given type.
   */
  bool is(ECycleType t);
    
  /** 
   * Turn off the cycle. 
   */
  void off();

  /**
   * Set the ambient light cycle.
   */
  void setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint8_t *intervals, ECycleLoop loop);
  void setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint16_t *intervals, ECycleLoop loop);
  void setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint32_t *intervals, ECycleLoop loop);
  
  /**
   * Set the cycle step.
   * previously CClyde::setCycleStep(uint8_t step)
   */
  void setCycleStep(uint8_t step);

  /**
   * Next step.
   * previously CClyde::nextStep(uint32_t now)
   */
  void nextStep(uint32_t now);

  /**
   * Speed up cycle.
   * previously CClyde::speedUpCycle(uint32_t factor)
   */
  void speedUp(uint32_t factor);

  /**
   * Stop the cycle. 
   */
  void stop();

private:

  /**
   * Set the ambient light cycle.
   */
  void setCycle(ECycleType type, uint8_t steps, const RGB* colors, ECycleLoop loop);
  
  /** 
   * Update the ambient light cycle.
   * previously CClyde::updateCycle()
   */
  void update();
  
  /**
   * Update the ambient light cycle to the next step.
   * previously CClyde::updateCycleNextStep(uint32_t now)
   */
  void updateNextStep(uint32_t now);
  
};


#endif
