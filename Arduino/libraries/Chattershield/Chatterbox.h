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


/* HERE IS WHERE I START TO PIECE TOGETHER THE NEW SHIELD FIRMWARE FROM 
  THE ARDUINO VS1003 LIB, FRANTZ'S CODE AND THE NEW SPI LIBRARY */

#ifndef __CLYDE_CHATTERBOX_H
#define __CLYDE_CHATTERBOX_H

#include <stdint.h>
  
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * The Chatterbox
 * VS1003
 * The mouth / speaker / mp3 player
 */
class Chatterbox {

public:  
  /** Constructor. */
  Chatterbox();

  /** Initialize Clyde. */
  void begin();
  
  /** Update Clyde's state */
  void update();

private:
  /** Detect the loudmouth shield. */
  void detectMouth();

};

#endif
