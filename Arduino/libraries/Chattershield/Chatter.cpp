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

#include "Chatter.h"

//oftwareSerial Chatter::mp3(CHATTER_RX_PIN, CHATTER_TX_PIN);

/**
 * Constructor
 */
Chatter::Chatter() {

  /*static const uint16_t ACK_TIMEOUT = 1000;

  detected = false;
  waitingOpCode = OP_NONE;
  lastCmdTime = 0;*/
}

bool Chatter::init(){
/*
  mp3.begin(9600);

  pinMode(CHATTER_SELECT_PIN, OUTPUT);
  pinMode(CHATTER_DETECT_PIN, INPUT);
  pinMode(CHATTER_RX_PIN, INPUT);
  pinMode(CHATTER_TX_PIN, OUTPUT);
  
  digitalWrite(CHATTER_SELECT_PIN, HIGH);
  */
}

void Chatter::detect() {
/*  #ifdef CLYDE_DEBUG
    Serial.println("Clyde: Trying to detect Loudmouth. Request set play mode: Single Loop Mode");
  #endif
  waitingOpCode = setPlayMode(PLAYMODE_SINGLE_CYCLE);
  */
}

void Chatter::update() {
 /* //detect the loudmouth shield at startup by waiting for mp3 player response
  if (!detected) {
    //only wait for a few seconds
    if (waitingOpCode == OP_NONE) {
      return;
    }
    else if (mp3.available() && PLAYMODE_SINGLE_CYCLE == mp3.read()) {
      #ifdef CLYDE_DEBUG
      Serial.println("Clyde: Loudmouth detected. Set play mode: Single Loop Mode.");
      #endif
      detected = true;
    }
    else if (lastCmdTime - millis() > ACK_TIMEOUT) {
      #ifdef CLYDE_DEBUG
      Serial.println("Clyde: Loudmouth not detected. Set play mode timeout.");
      #endif
      waitingOpCode = OP_NONE;
    }
  }
  else {
    //TODO
  }
  */
}

EOpCode Chatter::setPlayMode(EPlayMode playmode)
{
/*  mp3.write(0x7E);
  mp3.write(0x03);
  mp3.write(OP_SET_PLAY_MODE);
  mp3.write(playmode);
  mp3.write(0x7E);
  
  lastCmdTime = millis();
  
  return OP_SET_PLAY_MODE;
  */
}

EOpCode Chatter::play(uint16_t index)
{
  /*if (!detected) return OP_NONE;
  
  mp3.write(0x7E);
  mp3.write(0x04);
  mp3.write(OP_PLAY);
  mp3.write((index >> 8) & 0xFF);
  mp3.write(index & 0xFF);
  mp3.write(0x7E);
  
  lastCmdTime = millis();
  
  return OP_PLAY;*/
}

EOpCode Chatter::playState()
{
 /* if (!detected) return OP_NONE;

  mp3.write(0x7E);
  mp3.write(0x02);
  mp3.write(OP_PLAY_STATE);
  mp3.write(0x7E);
  
  lastCmdTime = millis();
  
  return OP_PLAY_STATE;
  */
}

EOpCode Chatter::setVolume(uint8_t volume)
{
 /* if (!detected) return OP_NONE;

  if (volume > 31) volume = 31;

  mp3.write(0x7E);
  mp3.write(0x03);
  mp3.write(OP_SET_VOLUME);
  mp3.write(volume);
  mp3.write(0x7E);
  
  lastCmdTime = millis();
  
  return OP_SET_VOLUME;
  */
}

EOpCode Chatter::pause(void)
{
 /* if (!detected) return OP_NONE;

  mp3.write(0x7E);
  mp3.write(0x02);
  mp3.write(OP_PAUSE);
  mp3.write(0x7E);
  
  lastCmdTime = millis();
  
  return OP_PAUSE;
  */
}

EOpCode Chatter::stop(void)
{
/*  if (!detected) return OP_NONE;

  mp3.write(0x7E);
  mp3.write(0x02);
  mp3.write(OP_STOP);
  mp3.write(0x7E);
  
  lastCmdTime = millis();
  
  return OP_PAUSE;
  */
}
