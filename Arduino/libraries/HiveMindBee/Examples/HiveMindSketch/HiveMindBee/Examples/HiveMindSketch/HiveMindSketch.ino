#include <Wire.h>
#include <EEPROM.h>
#include <HiveMindBee.h>
#include <SerialCommand.h>
#include <SoftwareSerial.h>

#define FIRMWARE_VERSION 1

SerialCommand sCmd;
int beeId = 1;
HiveMindBee Bee;

void setup() {

  Bee = HiveMindBee();
  Wire.begin();
  Bee.begin(beeId);
  
  Serial.begin(9600);
  // Uncomment this line to talk to Clyde over the Serial Monitor
  while (!Serial) ;
  Serial.println("HiveMind Bee is Ready!");
  
  initSerialCommands();
}

void loop() {
  //read the serial communication if any
  sCmd.readSerial();
  Bee.update();
}

boolean checkBeeId(int targetId){
  Serial.println("CHECK BEE ID");
  if(targetId == beeId)
    return true;
  else
    return false;
}

/*
#########################
 HiveMind Bee API
 #########################
 */

void initSerialCommands(){
  sCmd.addCommand("LIGHT_ON", cmdLightOn);
  sCmd.addCommand("LIGHT_OFF", cmdLightOff);
  sCmd.addCommand("SHIFT_COLOR", cmdShiftColor);
  sCmd.addCommand("START_CYCLE", cmdStartCycle);
  sCmd.addCommand("STOP_CYCLE", cmdStopCycle);
  sCmd.addCommand("PULSE", cmdPulse);
  sCmd.addCommand("DISPLAY_STRING", cmdDisplayString);
  sCmd.addCommand("CLEAR_DISPLAY", cmdClearDisplay);
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  Serial.println("init Serial Commands complete.");
}

/*
Light On
 **************
 Turn on the color light to specified color.
 
 Command:  LIGHT_ON <ID> <R> <G> <B>
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 R		  | The red value of RGB color, integer, 0-255
 G		  | The green value of RGB color, integer, 0-255
 B		  | The blue value of RGB color, integer, 0-255
 
 Return: OK
 */
void cmdLightOn() {
   Serial.println("LIGHT ON");
   
  char *param1, *param2, *param3, *param4;
  int targetId, r, g, b;

  //Get arguments
  param1 = sCmd.next();    // Id
  param2 = sCmd.next();    // Red
  param3 = sCmd.next();    // Green
  param4 = sCmd.next();    // Blue
  targetId = atoi(param1);
  r = atoi(param2);
  g = atoi(param3);
  b = atoi(param4);

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.lightOn(r, g, b);
    Serial.println("OK");
  }
}

/*
Light Off
 **************
 Turn off the color light.
 (Amanda's notes mention a parameter- handle on or pulsing...not sure what this is about....)
 
 Command:  LIGHT_OFF <ID>
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 
 Return: OK
 */
void cmdLightOff() {
  char *param1;
  int targetId;

  //Get arguments
  param1 = sCmd.next();    // Id
  targetId = atoi(param1);

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.lightOff();
    Serial.println("OK");
  }
}

/*
Shift Color
 **************
 Fade the color light to the specified color.
 (Amanda's notes state that this + speed could replace the cycle methods)
 
 Command:  SHIFT_COLOR <ID> <R> <G> <B>
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 R		  | The red value of RGB color, integer, 0-255
 G		  | The green value of RGB color, integer, 0-255
 B		  | The blue value of RGB color, integer, 0-255
 
 Return: OK
 */
void cmdShiftColor() {
  char *param1, *param2, *param3, *param4;
  int targetId, r, g, b;

  //Get arguments
  param1 = sCmd.next();    // Id
  param2 = sCmd.next();    // Red
  param3 = sCmd.next();    // Green
  param4 = sCmd.next();    // Blue
  targetId = atoi(param1);
  r = atoi(param2);
  g = atoi(param3);
  b = atoi(param4);

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.shiftColor(r, g, b);
    Serial.println("OK");
  }
}

/*
Start Cycle
 **************
 (Not sure what a cycle is in this instance.....)
 
 Command: START_CYCLE <ID> 
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 
 Return: OK
 */
void cmdStartCycle() {
  char *param1;
  int targetId;

  //Get arguments
  param1 = sCmd.next();    // Id
  targetId = atoi(param1);

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.startCycle();
    Serial.println("OK");
  }
}

/*
Stop Cycle
 **************
 (Not sure what a cycle is in this instance.....)
 
 Command: STOP_CYCLE <ID>
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 
 Return: OK
 */
void cmdStopCycle() {
  char *param1;
  int targetId;

  //Get arguments
  param1 = sCmd.next();    // Id
  targetId = atoi(param1);

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.stopCycle();
    Serial.println("OK");
  }
}

/*
Pulse
 **************
 Pulse, or blink,  the color light.
 
 Command: PULSE <ID> <R> <G> <B>
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 R		  | The red value of RGB color, integer, 0-255
 G		  | The green value of RGB color, integer, 0-255
 B		  | The blue value of RGB color, integer, 0-255
 
 Return: OK
 */

void cmdPulse() {
  char *param1, *param2, *param3, *param4;
  int targetId, r, g, b;

  //Get arguments
  param1 = sCmd.next();    // Id
  param2 = sCmd.next();    // Red
  param3 = sCmd.next();    // Green
  param4 = sCmd.next();    // Blue
  targetId = atoi(param1);
  r = atoi(param2);
  g = atoi(param3);
  b = atoi(param4);

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.pulse(r, g, b);
    Serial.println("OK");
  }
}
/*
Display String
 **************
 Display the specified string in the Bee's display
 
 Command: DISPLAY_STRING <ID> <STRING>
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 STRING	  | The string of text to display on Bee's display
 
 Return: OK
 */
void cmdDisplayString() {
  char *param1, *param2;
  int targetId;
  String text;

  //Get arguments
  param1 = sCmd.next();    // Id
  param2 = sCmd.next();    // Text String

  targetId = atoi(param1);
  text = param2;

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.displayString(text);
    Serial.println("OK");
  }
}

/*
Clear Display
 **************
 Clear any text from the Bee's display
 
 Command: CLEAR_DISPLAY <ID>
 
 Parameter | Description
 --------------------------------------
 ID 		  | The ID of the target Bee, integer
 
 Return: OK
 */
void cmdClearDisplay() {
  char *param1;
  int targetId;

  //Get arguments
  param1 = sCmd.next();    // Id
  targetId = atoi(param1);

  //If this is the target bee.
  if(checkBeeId(targetId)){
    Bee.clearDisplay();
    Serial.println("OK");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.println("What?");
}
