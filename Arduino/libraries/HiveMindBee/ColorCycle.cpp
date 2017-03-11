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

#include "ColorCycle.h"

/**
 * Constructor
 */
ColorCycle::ColorCycle() {

  //init ambient cycle
  type = OFF;
  numSteps = 0;
  step = 0;
  stepStart = 0;
  stepEnd = 0;
  stepColor = RGB(0, 0, 0);
  memset((void*)&colors[0], 0, sizeof(RGB)*ColorCycle::MAX_CYCLE_LENGTH);
  memset((void*)&intervals[0], 0, sizeof(uint32_t)*ColorCycle::MAX_CYCLE_LENGTH);
  loop = NO_LOOP;

}

bool ColorCycle::init(ColorLight* l){
  ambientLight = l;
}

/**
 * Check if there is an ongoing cycle.
 */
bool ColorCycle::isOn() { return type != OFF; }
    
/**
 * Check if the current cycle matches a given type.
 */
bool ColorCycle::is(ECycleType t) { return t == type; }
    
/** 
 * Turn off the cycle. 
 */
void ColorCycle::off() { type = OFF; }


void ColorCycle::setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint8_t *intervals, ECycleLoop loop) {
  for(int i = 0; i < steps; i++)
    this->intervals[i] = *(intervals + i);
  
  setCycle(type, steps, colors, loop); 

}
void ColorCycle::setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint16_t *intervals, ECycleLoop loop) {
 for(int i = 0; i < steps; i++)
    this->intervals[i] = *(intervals + i);
  
  setCycle(type, steps, colors, loop);  

}
void ColorCycle::setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint32_t *intervals, ECycleLoop loop) {
  for(int i = 0; i < steps; i++)
    this->intervals[i] = *(intervals + i);
  
  setCycle(type, steps, colors, loop);  
}

void ColorCycle::setCycle(ECycleType type, uint8_t steps, const RGB *colors, ECycleLoop loop) {
  this->type = type;
  this->numSteps = steps;
  
  for(int i = 0; i < steps; i++)
    this->colors[i] = *(colors+i);
  
  this->step = 0;
  this->stepStart = millis();
  this->stepEnd = this->stepStart + this->intervals[this->step];
  this->stepColor = RGB(ambientLight->color.r, ambientLight->color.g, ambientLight->color.b);
  this->loop = loop;

} 

void ColorCycle::stop() {  //TODO should this be a function pointer set when starting cycle
  switch(type) {
    case SUNSET:
      //if current cycle is the afraid of the dark sunset,
      //then speed up and complete cycle
      //TODO setCycle could also set a pointer to a stop function...
      speedUp(100);
      break;
    default:
      //for any other cycle, turn it off
      ambientLight->turnOff();
      off();  
      break;
  } 
}

/*
void CClyde::blink(const RGB& rgb, uint32_t onDuration, uint32_t offDuration, uint8_t numBlinks) {
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

/**
 * Update the cycle.
 * previously CClyde::updateCycle()
 */
void ColorCycle::update() {
  uint32_t now = millis();
  
  //if we reach the end of the current step
  if (now > this->stepEnd) {
    //process to next step
    updateNextStep(now);
    //if we reached the end, then we're done
    if (!this->isOn())
      return;
  }

  //find where we are in the step 0-1
  float t;
  t = this->stepEnd - this->stepStart;
  t = t == 0 ? 1 : (now - this->stepStart) / t;

  //calculate the color for t
  RGB newColor;
  int16_t diff;
  diff = this->colors[this->step].r - this->stepColor.r;
  newColor.r = this->stepColor.r + (t*diff);
  diff = this->colors[this->step].g -this->stepColor.g;
  newColor.g = this->stepColor.g + (t*diff);
  diff = this->colors[this->step].b - this->stepColor.b;
  newColor.b = this->stepColor.b + (t*diff);
  
  ambientLight->setColor(newColor);
}

/**
 * Update the cycle to the next step.
 * previously CClyde::updateCycleNextStep(uint32_t now)
 */
void ColorCycle::updateNextStep(uint32_t now) {
  this->step++;

  //stop cycle if we reached the end
  if (this->step >= this->numSteps) {
    if (this->loop == LOOP) {
      this->step = 0;
    }
    else {
      this->type = OFF;
      ambientLight->setColor(this->colors[this->numSteps-1]);
      this->stop();
      return;
    }
  }

  this->stepStart = now;
  this->stepEnd += this->intervals[this->step];
  this->stepColor = RGB(ambientLight->color.r, ambientLight->color.g, ambientLight->color.b);
}

/**
 * Set the cycle step.
 * previously CClyde::setCycleStep(uint8_t step)
 */
void ColorCycle::setCycleStep(uint8_t step) {
  if (step >= this->numSteps) return;
  
  this->step = step;
  this->stepStart = millis();
  this->stepEnd += this->intervals[this->step];
}

/**
 * Next step.
 * previously CClyde::nextStep(uint32_t now)
 */
void ColorCycle::nextStep(uint32_t now) {
  this->stepEnd = now;
  updateNextStep(now);
}

/**
 * Speed up cycle.
 * previously CClyde::speedUpCycle(uint32_t factor)
 */
void ColorCycle::speedUp(uint32_t factor) {
  //reduce interval time to speed up end of cycle
  for(int i = 0; i < this->numSteps; i++) {
    this->intervals[i] /= factor;
    factor *= 2;
  }
  
  //jump cycle to next color
  nextStep(millis());
}
