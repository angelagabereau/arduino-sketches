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

#include "ClydeEEPROM.h"

#include "FabuleClydeDev.h"
#include "EEPROM.h"
#include <string.h>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define CLYDE_SERIAL_LENGTH 6

void ClydeEEPROM::writeSerial(char *serial) {
  for(int i = 0; i < CLYDE_SERIAL_LENGTH; i++)
    //verify hexadecimal string
    //TODO replace with 'characters', clearer
    if (*(serial + i) < 48 || *(serial + i) > 70 ||
        (*(serial + i) > 59 && *(serial + i) < 65))
      return;
  
  for(int i = 0; i < CLYDE_SERIAL_LENGTH; i++)
    EEPROM.write(i, *(serial+i));
}

void ClydeEEPROM::readSerial(char* serial) {
  for(int i = 0; i < CLYDE_SERIAL_LENGTH; i++)
    *(serial + i) = EEPROM.read(i);
}

void ClydeEEPROM::writeQC(bool qc) {
  EEPROM.write(QC_ADDR, qc ? 1 : 0);
}
  
void ClydeEEPROM::readQC(bool* qc) {
  *(qc) = EEPROM.read(QC_ADDR) == 1;
}

void ClydeEEPROM::writeAmbientColor(RGB* color) {
  EEPROM.write(AMBIENT_ADDR + 0, color->r);
  EEPROM.write(AMBIENT_ADDR + 1, color->g);
  EEPROM.write(AMBIENT_ADDR + 2, color->b);
}

void ClydeEEPROM::readAmbientColor(RGB* color) {
  *color = RGB(
    EEPROM.read(AMBIENT_ADDR + 0),
    EEPROM.read(AMBIENT_ADDR + 1),
    EEPROM.read(AMBIENT_ADDR + 2)
  );
}
/*
bool ClydeEEPROM::clearSunsetCycle() {
  RGB sunsetColors[CClyde::CAmbientCycle::MAX_CYCLE_LENGTH];
  memset( (void*)&sunsetColors[0], 0, sizeof(RGB)*CClyde::CAmbientCycle::MAX_CYCLE_LENGTH);
  
  uint32_t sunsetIntervals[CClyde::CAmbientCycle::MAX_CYCLE_LENGTH];
  memset( (void*)&sunsetIntervals[0], 0, sizeof(uint32_t)*CClyde::CAmbientCycle::MAX_CYCLE_LENGTH);

  return writeSunsetCycle(CClyde::CAmbientCycle::MAX_CYCLE_LENGTH, sunsetColors, sunsetIntervals);
}

bool ClydeEEPROM::writeSunsetCycle(uint8_t steps, RGB *colors, uint32_t *intervals)
{
  //make sure that we don't write a cycle with more steps than we have space for
  if (steps > CClyde::CAmbientCycle::MAX_CYCLE_LENGTH)
    return false;
  
  uint16_t addr = SUNSET_ADDR;
  
  //write sunset cycle
  EEPROM.write(addr, steps);
  
  addr++;
  for(int i = 0; i < steps; i++) {
    EEPROM.write(addr + i*3 + 0, (*(colors + i)).r);
    EEPROM.write(addr + i*3 + 1, (*(colors + i)).g);
    EEPROM.write(addr + i*3 + 2, (*(colors + i)).b);
  }
    
  addr += steps*3;
  for(int i = 0; i < steps; i++) {
    EEPROM.write(addr + i*4, (*(intervals + i) >> 24) & 0xFF);
    EEPROM.write(addr + i*4 + 1, (*(intervals + i) >> 16) & 0xFF);
    EEPROM.write(addr + i*4 + 2, (*(intervals + i) >> 8) & 0xFF);
    EEPROM.write(addr + i*4 + 3, *(intervals + i) & 0xFF);
  }
  
  return true;
}

bool ClydeEEPROM::readSunsetCycle(uint8_t* steps, RGB *colors, unsigned long *intervals)
{
  //read sunset cycle eeprom address
  unsigned short sunsetAddr = SUNSET_ADDR;
  
  //read sunset cycle
  uint8_t s = EEPROM.read(sunsetAddr);
  
  //make sure that we don't read a cycle with more steps than we have space for
  if (s > CClyde::CAmbientCycle::MAX_CYCLE_LENGTH)
    return false;
    
  //read cycle
  *(steps) = s;
  
  sunsetAddr++;
  for(int i = 0; i < *(steps); i++) {
    (*(colors + i)).r = EEPROM.read(sunsetAddr + i*3 + 0);
    (*(colors + i)).g = EEPROM.read(sunsetAddr + i*3 + 1);
    (*(colors + i)).b = EEPROM.read(sunsetAddr + i*3 + 2);
  }
    
  sunsetAddr += *(steps)*3;
  for(int i = 0; i < *(steps); i++) {
    *(intervals + i) = EEPROM.read(sunsetAddr + i*4);
    *(intervals + i) = (*(intervals + i) << 8) | EEPROM.read(sunsetAddr + i*4 + 1);
    *(intervals + i) = (*(intervals + i) << 8) | EEPROM.read(sunsetAddr + i*4 + 2);
    *(intervals + i) = (*(intervals + i) << 8) | EEPROM.read(sunsetAddr + i*4 + 3);
  }
    
  return true;
}
*/
void ClydeEEPROM::reset() {
  //char serial[7] = "012345";
  uint16_t ver = 1;
  RGB ambient(20,255,54);
  
  //writeSerial(&serial[0]);
  //writeQC(false);
  writeAmbientColor(&ambient);
  
  //write the default sunset cycle
  //TODO: move values to somewhere more obvious, like top of ClydeAfraidOfTheDark.h
  const uint8_t sunsetSteps = 9;
  RGB sunsetColors[sunsetSteps] = {RGB(255, 150, 0), RGB(255, 0, 0), RGB(100, 0, 200), RGB(0, 0, 255), RGB(0, 0, 0), RGB(0, 0, 180), RGB(0, 0, 0), RGB(0, 0, 125), RGB(0, 0, 0)};
  uint32_t sunsetIntervals[sunsetSteps] = {3000, 400000, 300000, 200000, 180000, 120000, 3000, 2000, 1000}; //real
  //uint32_t sunsetIntervals[sunsetSteps] = {500, 10000, 5000, 5000, 3000, 2000, 1500, 1000, 1000}; //demo
  
  clearSunsetCycle();
  writeSunsetCycle(sunsetSteps, &sunsetColors[0], &sunsetIntervals[0]);
}