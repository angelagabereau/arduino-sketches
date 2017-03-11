/*
  Clyde Tests
  Chatterbox

  Test Chatterbox playback functions. Play each song in the repertoire.
  To see debug message: In ClydeV2_DebugFlags.h, uncomment #define CLYDE_DEBUG_CHATTERSHIELD 
  and open the Serial Monitor.

  This example code is in the public domain.

  modified 16 Feb 2015
  by Angela Gabereau
 */

#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <MPR121.h>
#include <ClydeV2.h>

#define FIRMWARE_VERSION 2

void setup(){
  
  Wire.begin();
  Clyde.begin();

  Serial.begin(9600); // start serial for debugging output
  delay(2000);
  
  Serial.println("Clyde is Ready!");
  Serial.println("Clyde Chatterbox");
  Serial.println();

  Clyde.chatterbox.playOn();
  delay(2000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playOff();
  delay(2000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
    
  Clyde.chatterbox.playHappy();
  delay(20650);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playLaugh();
  delay(9000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playError();
  delay(600);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playClock1h();
  delay(24000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playClock15m();
  delay(6000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playClock30m();
  delay(12000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playClock45m();
  delay(18000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playClockChime();
  delay(24000);
  Clyde.chatterbox.pause();
 // Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playNotification();
  delay(1200);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playLullaby();
  delay(44000);
  Clyde.chatterbox.pause();
  //Clyde.chatterbox.stop();
  
  Clyde.chatterbox.playDaisy();
  delay(36000);
  Clyde.chatterbox.pause();
  
 //Clyde.chatterbox.stop();
 //Clyde.chatterbox.pause();
}

void loop() {
}
