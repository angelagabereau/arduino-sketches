#include <Wire.h>
#include <SPI.h>
#include <Chattershield.h>

CChattershield chattershield(4, 12, 13); // cs_pin, dcs_pin, dreq_pin, reset_pin

void setup(){
  
  Wire.begin();
  
  Serial.begin(9600); // start serial for debugging output
  Serial.println("Chattershield!");
  Serial.println("Chattershield Dev");
  Serial.println();
  
  chattershield.begin();
  delay(2000);
  
  chattershield.setVolume(0x00);
  chattershield.playTrack(12);
  delay(1000);
  chattershield.fadeOut();

}

void loop() {
}
