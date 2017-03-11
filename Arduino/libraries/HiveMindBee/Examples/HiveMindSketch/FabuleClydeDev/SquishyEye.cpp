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
#include "debugflags.h"
#include "SquishyEye.h"
#include "clydepins.h"


/**
 * Constructor
 */
SquishyEye::SquishyEye() {

}

bool SquishyEye::init(){
    //init eye
  pin = EYE_PIN;
  onceCalibrated = false;
  isCalibrated = false;
  //calibBlink = true;
  //nextCalibBlink = 3000;
  calibLock = 0;
  calibCount = 0;
  irValue = 0;
  irMin = 1025;
  irMax = 0;
  irThreshold = 0;
  irLast = 0;
  pressedLast = 0;
  pressedStart = 0;
  pressedCount = 0;
  pressDetectLock = 0;
  
#ifdef CLYDE_DEBUG_EYE
  restartCount = 0;
#endif

}

void SquishyEye::update() {

// Serial.print("SquishyEye::update()");

  // Calibrated the eye's IR sensor
  calibrate();

  // Check if the eye is pressed
  if(onceCalibrated)
    isPressed();
}

void SquishyEye::calibrate() {

 
  // In order for the SquishyButton to function we must calibrate the button
  // to work in Clyde's current lighting conditions.  To do this we will collect
  // a series of ir sensor values and determine the max and min ir sensor values.
  // From these we have a profile of the SquishyButton's resting state.  From this 
  // resting state we determine a threshold value, irThreshold 
  // A button press is detected by comparing current ir sensor values to irThreshold 

  //Do not proceed with calibration, if calibration is locked.
  if (millis() < calibLock) return;

  //Read the IR sensor's current
  irValue = analogRead(pin);

  //If this is the first time through, set the irLast to the current irValue
  if(irLast ==0){
     // irLast = irValue;
  }

  #ifdef CLYDE_DEBUG_EYE
 // Serial.print("Clyde: IR = ");
 // Serial.print(irValue);
  #endif

  //if IR has never been calibrated, blink white light until it is
  /*if(!onceCalibrated) {
    long now = millis();
    if (now > 2000 && (now < 12000 || Clyde.white()->isOn())) {
      setWhite(255 - ((sin((now - 2000)/500.0 - HALF_PI) + 1) / 2.0 * 20));
    }
    else if (now >= 12000) {
      setWhite(0);
      m_eye.onceCalibrated = true; //TODO: use a better variable for this
    }
  } */
  
  //If the eye is pressed, don't try to calibrate
  if (pressedCount > 0) return;

  //Check if the IR sensor values are stable.
  
  //Get difference between the current and the last ir sensor value. 
  int32_t irDiff = irValue > irLast ? irValue - irLast : irLast - irValue;

 #ifdef CLYDE_DEBUG_EYE
  //Serial.print(" IR last ");
 // Serial.print(irLast);
 // Serial.print("Clyde: IR diff = ");
 // Serial.println(irDiff);

  #endif

  //If the difference is within the accepted range then collect current ir sample.
  if (irDiff < CALIB_MAX_RANGE) {
    calibCount++;
    irMin = irValue < irMin ? irValue : irMin;
    irMax = irValue > irMax ? irValue : irMax;
  }
  //if the value is too far from the previous, restart calibration
  else {
    calibCount = 0;
    irMin = irMax = irValue;
   // resetCalibration();
    return;
  }

  //Once we've collected enough sample ir values we can set the button's resting state.
  if (calibCount >= CALIB_NUM_REPEATS) {
    
    //Check if the range of ir values within the acceptable range size.  If not, restart calibration.
    if (irMax-irMin >= CALIB_MAX_RANGE) {
      resetCalibration();
      return;
    }
    

    //Calculate the threshold ir sensor value for the SquishyButton.
    //When the ir sensor value reaches the button is pressed, and we can trigger onPress behaviors.

    //Find the middle of the resting state range.
    uint16_t irAvg = (irMin + irMax) / 2;
    
    //only calibrate if the threshold will be above a certain limit
    //if not it's too unpredictable (e.g. the sun is shining on it)
    if (irAvg < (uint16_t)((CALIB_FORMULA_B - CALIB_MIN_THRESHOLD_DIFF) / CALIB_FORMULA_A)) {
      //if the eye was not calibrated, turn on ambient light to show feedback
    /*  if (!calibrated)
        fadeAmbient(savedColor, 0.1f);
      
      if (!onceCalibrated)
        setWhite(255);
*/

     

      isCalibrated = true;
      onceCalibrated = true;

      //calculate the threshold
      //simple conversion from detected base ir to threshold
      //the less ir detected (higher value) the less difference required to trigger
      uint16_t newThreshold = irAvg * CALIB_FORMULA_A + CALIB_FORMULA_B;

      #ifdef CLYDE_DEBUG_EYE
        if (irThreshold != newThreshold) {
          Serial.print("Clyde: eye calibrated. avg = ");
          Serial.print(irAvg);
          Serial.print(", threshold = ");
          Serial.print(newThreshold);
          Serial.print(", range = ");
          Serial.print(irMax - irMin);
          Serial.print(", noisy restarts = ");
          Serial.println(restartCount);
        }
        restartCount = 0;
      #endif      
     
      irThreshold = newThreshold;
    }
    //if there's NOT enough IR emitted by the circuit to recalibrate, then set to recalibrate
    else if (isCalibrated) {

      resetCalibration();
      //blink(RGB(255, 0, 0), 200, 200, 3);
      //setAmbient(RGB(0, 0, 0));
      //setWhite(255);
      
      //setPlayMode(PLAYMODE_SINGLE);
      //play(SND_ERROR);
      
      #ifdef CLYDE_DEBUG
      Serial.print("Clyde: eye uncalibrated. not enough IR detected, check circuit. ir = ");
      Serial.print(irAvg);
      Serial.print(", minimum = ");
      Serial.println((uint16_t)((CALIB_FORMULA_B - CALIB_MIN_THRESHOLD_DIFF) / CALIB_FORMULA_A));
      #endif
    }
    

  }

  irLast = irValue;


}


void SquishyEye::resetCalibration(){

   //Serial.println("SquishyEye::resetCalibration()");

    //reset values
    isCalibrated = false;
    calibCount = 0;
    irMin = 1025;
    irMax = 0;

    #ifdef CLYDE_DEBUG_EYE
      restartCount++;
    #endif

}

bool SquishyEye::isPressed() {

   

    // Button presses are detected by comparing the current ir sensor value to
    // the threshold that was set during calibration.  In order to prevent false positives
    // we will check that the threshold had been reached in multiple sequential sensor readings.
    // Once the threshold has been reached the required number of times, the button press
    // event handling is triggered.


    //Is the calibration locked?
    //Is the button press check locked?
    //Do not check for button press until the button has been calibrated and unless 
    //enough time has passed since Clyde board started.
    if (!isCalibrated || millis() < pressDetectLock) return false;

    //What is the current IR sensor value?
    //Read the current value of the IR sensor
    irValue = analogRead(pin);

    #ifdef CLYDE_DEBUG_EYE
   // Serial.print("Clyde: IR = ");
   // Serial.print(irValue);
   // Serial.print("Clyde: IR threshold = ");
   // Serial.println(irThreshold);
    #endif


      //Is the button active  aka. does the current IR value meet the threshold
    if (irValue >= irThreshold) {

        //The button is active

        //Increment the press counter and store the current time
        pressedCount++;
        pressedLast = millis();

        //If the button is active for a given number of times, we consider it a button press.
        if (pressedCount == PRESS_COUNT_THRESHOLD) {

          //Let's check if the button press has already been detected,
          //but not released.  If enough time has passed, auto release 
          if (millis() > pressedStart + PRESS_RELEASE_TIMER) {

            //We need to check and make sure that the user has not continued to press after we have triggered the press event.
            //If it is still pressed, lock the press detection and calibraion locks, and hope that the user takes their hand off
            //the button before we try to detect the button press again.
            autoRelease();

          }else{

              //The button is pressed!
              onPress();
              return true;
          }
        
        }


      }else if (millis() > pressedLast+200){
        //The button is not pressed, and it's been a bit of time, 
        // then reset pressedCount to get ready for the next press
        pressedCount = 0;
      }

  return false;
}

void SquishyEye::onPress(){

    #ifdef CLYDE_DEBUG_EYE
    Serial.print("SquishyEye::onPress()");
    #endif

    //Store the time of this press
    pressedStart = pressedLast; 

    //Call the press event handler.
    if(pressedHandler) pressedHandler();
}

void SquishyEye::onRelease(){

    #ifdef CLYDE_DEBUG_EYE
    Serial.print("SquishyEye::OnRelease()");
    #endif

    //Reset values
    pressedLast = 0;
    pressedStart = 0;
    pressedCount = 0;

    //Call the release event handler.
    if(releasedHandler) releasedHandler();
}

void SquishyEye::autoRelease(){

    //Put on the locks!  We are autoreleasing, but the button is still pressed, 
    //we do not want to calibrate or detect button presses with the button 
    //already pressed.
    calibLock = pressDetectLock = millis() + LOCK_TIMER;
    onRelease();

    #ifdef CLYDE_DEBUG_EYE
    Serial.println("Clyde: eye long press detected. auto release.");
    #endif
}

    /*

  // check if it is time to calibrate or not
  if(millis()-lastSampled < TIME_THRESH) return; 
  
  // let's read the sensor now
  previousIrRaw = currentIrRaw;
  currentIrRaw = analogRead(EYE_PIN);
  
  #ifdef CLYDE_DEBUG_EYE
  Serial.print("current ir raw: ");
  Serial.print(currentIrRaw);
  Serial.print(" previous ir raw: ");
  Serial.println(previousIrRaw);
  #endif

  // check if the eye is pressed -- don't calibrate if it is!
  if(isPressed == true && pressThresh > 0) return;
  
  // check what the delta is- if it is not stable, we will try again next time
  uint16_t rawDelta = 0;

  if(previousIrRaw != currentIrRaw) rawDelta = math_abs(previousIrRaw, currentIrRaw);
  
//The raw ir values are not stable, we cannot calibrate until they are, return.
if(rawDelta >= DELTA_THRESH) {
  #ifdef CLYDE_DEBUG_EYE
  //  Serial.print("too much delta in raw vals:");
  //  Serial.println(rawDelta);
  #endif

  return;
}
  
  // capture the min and max vals
  if(sampleCount == 0) {
    irMinRaw = currentIrRaw;
    irMaxRaw = currentIrRaw;
  }
  
  if(currentIrRaw < irMinRaw) irMinRaw = currentIrRaw;
  if(currentIrRaw > irMaxRaw) irMaxRaw = currentIrRaw;
  
  // increment
  irTotal += currentIrRaw;
  sampleCount++;
  
  #ifdef CLYDE_DEBUG_EYE
 // Serial.print("sample count: ");
 // Serial.println(sampleCount);
  #endif

  // now it is time to calculate the average val
  if(sampleCount >= SAMPLE_SIZE) {
    
    // calculate average
    previousIrAvg = currentIrAvg;
    float temp = (float)( (float)(irTotal) / SAMPLE_SIZE);
    currentIrAvg = (uint16_t)temp;
    
    float temp2 = (float)((float)(irMinRaw + irMaxRaw) / 2 );
    irRangeAvg = temp2;
    
    
    #ifdef CLYDE_DEBUG_EYE
    Serial.print("current ir val: ");
    Serial.println(currentIrAvg);

    Serial.print("ir min: ");
    Serial.print(irMinRaw);
    Serial.print(" ir max: ");
    Serial.println(irMaxRaw);
    #endif
    
    // reset!
    irTotal = 0;
    sampleCount = 0;
    
    
    // check what the range is- if it is not stable, we will try again next sample
    // commented out as we do not need it -- but we left it in case you need it
    
    uint16_t rangeDelta = math_abs(irMinRaw, irMaxRaw);
    
    if(rangeDelta >= RANGE_THRESH) {
      #ifdef CLYDE_DEBUG_EYE
      Serial.print("range is not stable: ");
      Serial.println(rangeDelta);
      #endif

      return;
    }
    
    
    
    // check what the delta is- if it is not stable, we will try again next sample
    // commented out as we do not need it -- but we left it in case you need it!
    
    uint16_t avgDelta = 0;

    if(currentIrAvg != previousIrVal) avgDelta = math_abs(currentIrAvg, previousIrVal);
    
    if(avgDelta >= DELTA_THRESH) {
      #ifdef CLYDE_DEBUG_EYE
      Serial.print("avg delta is not stable: ");
      Serial.println(avgDelta);
      #endif
    }
    
    
    // calculate the new press theshold
    pressThresh = currentIrAvg + PRESS_SENSITIVITY;
    
    
    // we are done now
    doneCalibration = true;
    lastCalibrated = millis();
    
  }
  
  lastSampled = millis();

  */

  /*
    #ifdef CLYDE_DEBUG_EYE
  Serial.print("is Button Pressed");
  Serial.print(currentIrRaw);
  Serial.print(" pressThresh: ");
  Serial.print(pressThresh);
  Serial.print(" isPressed: ");
  Serial.print(isPressed);
  #endif

  // do we detect a press, when isPressed is not set yet?
  if(currentIrRaw > pressThresh && isPressed == false) {
    
    #ifdef CLYDE_DEBUG_EYE
    Serial.print("detected press ");
    Serial.println(pressSampleCount);
    #endif

    // reset the counter if this is the first press in quite some time
    if(millis()-lastPress >= PRESS_LAST_RESET && lastPress != 0) {
      pressSampleCount = 0;
    }
    
    // increment and record the press...
    pressSampleCount++;
    lastPress = millis();
    
    //The button is pressed!
    // if we exceed the specific number of presses, then set isPressed!
    if(pressSampleCount > PRESS_COUNT_THRESH) {
      
      pressStart = millis();
      isPressed = true;
      pressSampleCount = 0;

      #ifdef CLYDE_DEBUG_EYE
      Serial.println("pressed!");
      #endif
    }
    
  }
  
  
  // do we detect a press, when isPressed is set?
  if(currentIrRaw > pressThresh && isPressed == true) {

      #ifdef CLYDE_DEBUG_EYE
      Serial.print("pressed!");
      Serial.print(currentIrRaw);
      Serial.print(" > ");
      Serial.println(pressThresh);
      #endif
     
    lastPress = millis();

    if(pressedHandler) pressedHandler();
    
  }
  

  // is it pressed? we should check if it is still pressed
  if(isPressed == true) {
   
    // check that the press is not timed out yet -- otherwise, return true
    // alternatively, if you want the press to time out after a certain
    // amount of time, you can do something like this:
    //if(millis()-pressStart >= pressStart_TIMEOUT 
    if(millis()-lastPress >= PRESS_LAST_TIMEOUT) {
      //if(LOG_LEVEL <= DEBUG) *debug_stream << "press time's up" << endl;
      isPressed = false;
      lastPress = 0;

      if(releasedHandler) releasedHandler();

      return false;
    }
    
    return true;
    
  }
  
  return false;  
  */

    //Once the press threshold state has been detected multiple times, we consider 
  //if the eye press is detected enough time, trigger press event
 /* if (pressedCount == PRESS_COUNT_THRESHOLD) {

    //and we detect that's it's still pressed,
    //then keep track of the last time is was detected
    if (irValue >= irThreshold) {
      pressedLast = millis();

      //if the eye has been pressed for a some time, auto release
      if (millis() > pressedStart + PRESS_RELEASE_TIMER) {

        //Put on the locks!  We are autoreleasing, but the button is still pressed, 
        //we do not want to  calibrate or detect button presses with the button 
        //already pressed.
        calibLock = pressLock = millis() + LOCK_TIMER;
        onRelease();

        //blink(RGB(255,0,0), 200, 200, 3);
        // setAmbient(RGB(0, 0, 0));
        // setWhite(255);
          
        // setPlayMode(PLAYMODE_SINGLE);
        //play(SND_ERROR);
  
        #ifdef CLYDE_DEBUG_EYE
        Serial.println("Clyde: eye long press detected. auto release.");
        #endif
      }
    }
    // if it's not pressed, and it's been a bit of time, 
    // then reset pressedCount to get ready for the next press
    else if (millis() > pressedLast+200) {
      pressedCount = 0;
    }
  }
  //if the eye is pressed, increase detection count
   pressedCount++;
    pressedLast = millis();

    //We've got a button press!
    if (pressedCount == PRESS_COUNT_THRESHOLD) {
      pressedStart = pressedLast;
      //The button is pressed....
      onPress();
      return true;
    }
  }
  //if it's been some time since the last detection of a released state, then release
  else if (millis() > pressedLast+500) {
    pressedCount = 0;
  }
*/

  /*
  // -- eye / ir sample related
  lastCalibrated = 0;
  lastSampled = 0;
  sampleCount = 0;
  irTotal = 0;
  irRangeAvg = 0;
  doneCalibration = false;

  // the raw, un-averaged, ir vals from analog in
  currentIrRaw = 0;
  previousIrRaw = 0;
  irMinRaw = 0;
  irMaxRaw = 0;

  // the averaged reading ir vals over time
  currentIrAvg = 0;
  previousIrAvg = 0;

  // eye button press related
  pressThresh = 0;
  pressStart = 0;
  isPressed = false;
  pressSampleCount = 0;
  lastPress = 0;
  */
