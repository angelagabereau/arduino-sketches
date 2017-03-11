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

#include "Chatterbox.h"
#include "Chatter.h"
#include "Chattershield.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//Chatterbox Chatterbox; //The Clyde Singleton


/**
 * Constructor
 */
Chatterbox::Chatterbox() {
    // Chatter chatter = Chatter();

}
void Chatterbox::begin() {

  //Initialize the chatter audio module
 // chatter.init();

}

void Chatterbox::update(){

}

void Chatterbox::detectMouth() {
 /* #ifdef CLYDE_DEBUG
    Serial.println("Clyde: Trying to detect Loudmouth. Request set play mode: Single Loop Mode");
  #endif
  m_mouth.waitingOpCode = Clyde.setPlayMode(PLAYMODE_SINGLE_CYCLE);*/
}
