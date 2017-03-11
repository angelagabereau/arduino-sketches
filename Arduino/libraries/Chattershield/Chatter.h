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
#ifndef __CLYDE_CHATTER_H
#define __CLYDE_CHATTER_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * Enum types for mp3 op codes.
 */
enum EOpCode {
  //TODO check WT5001 for other useful features to implement if space
  OP_NONE = 0,
  OP_PLAY = 0xA0,
  OP_PAUSE = 0xA3,
  OP_STOP = 0xA4,
  OP_SET_VOLUME = 0xA7,
  OP_SET_PLAY_MODE = 0xA9,
  OP_SET_DATE = 0xB1,
  OP_SET_TIME = 0xB2,
  OP_PLAY_STATE = 0xC2
};

/**
 * Enum types for mp3 play modes.
 */
enum EPlayMode {
  PLAYMODE_SINGLE = 0x00,
  PLAYMODE_SINGLE_CYCLE = 0x01,
  PLAYMODE_ALL_CYCLE = 0x02,
  PLAYMODE_RANDOM = 0x03
};

/**
 * Enum types of the sound file indexes for Loudmouth
 */
enum EAudioIndex {
  SND_ON = 1,
  SND_OFF = 2,
  SND_HAPPY = 3,
  SND_LAUGH = 4,
  SND_ERROR = 5,
  SND_CLOCK_1H = 6,
  SND_CLOCK_15MIN = 7,
  SND_CLOCK_30MIN = 8,
  SND_CLOCK_45MIN = 9,
  SND_CLOCK_CHIME = 10,
  SND_NOTIFICATION = 11,
  SND_AU_CLAIR_DE_LA_LUNE = 12,
  SND_DAISY_BELL = 13
};

/**
 * The Chatter Audio Shield
 * The mouth / speaker / mp3 player
 */
class Chatter {

    static const uint16_t ACK_TIMEOUT = 1000;

    bool detected;
    EOpCode waitingOpCode;
    uint32_t lastCmdTime;
    
    //static SoftwareSerial mp3;

public:  
  /** Constructor. */
  Chatter();
  
  /**
   * Initialize 
   */  
  bool init();
  /**
   * previously CClyde::detectMouth();
   */
  void detect();
  /**
   * previously CClyde::updateMouth();
   */
  void update();

 /**
   * Set the loudmouth mp3 player play mode.
   */
  EOpCode setPlayMode(EPlayMode playmode);
  
  /**
   * Play a file using the Loudmouth shield.
   */
  EOpCode play(uint16_t index);
  
  /**
   * Get the play state of the Loudmouth shield.
   */
  EOpCode playState();
  
  /**
   * Set volume of the Loudmouth shield.
   * @volume level 0 - 31
   */
  EOpCode setVolume(uint8_t volume);  
  
  /**
   * Pause the audio of the Loudmouth shield.
   */
  EOpCode pause(void);

  /**
   * Stop the audio of the Loudmouth shield.
   */
  EOpCode stop(void);

private:
  
};

#endif