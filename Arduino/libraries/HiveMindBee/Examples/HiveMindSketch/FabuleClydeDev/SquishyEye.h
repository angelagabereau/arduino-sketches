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
#ifndef __CLYDE_SQUISHYEYE_H
#define __CLYDE_SQUISHYEYE_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * Squishy Eye Button
 * The squishy eye.
 */
class SquishyEye {

      //static const uint16_t CALIB_BLINK_INTERVAL = 3000;  /**< Time between white light blinks during first calibration in millis. */
    //static const uint16_t CALIB_BLINK_DURATION = 100;   /**< Time of blinks during first calibration in millis. */
    static const uint8_t CALIB_SAMPLE_INTERVAL = 10;      /**< Time between calibration samples in millis. */
    static const uint8_t CALIB_MAX_RANGE = 40;            /**< Maximum value range accepted for calibration. */
    static const uint8_t CALIB_NUM_REPEATS = 125;         /**< Numbers of samples to check for calibration. */
    static const uint16_t CALIB_IR_BASE = 610;            /**< Base IR reading without any IR from outside. */
    static const float CALIB_FORMULA_A = 0.5;             /**< Multiplier for calibration formula. */
    static const uint16_t CALIB_FORMULA_B = 450;          /**< Base for calibration formula. */
    static const uint16_t CALIB_MIN_THRESHOLD_DIFF = 50;  /**< Minimum difference between base and threshold. */    
    static const uint16_t CALIB_MAX_IR = 100;             /**< Maximum IR needed for calibration. Anything more means too much noise/sun. */
    static const uint8_t PRESS_COUNT_THRESHOLD = 8;       /**< Number of press detection needed to trigger a press event. */
    static const uint8_t PRESS_RELEASE_TIMER = 3000;      /**< The number of milliseconds after a button press that we will auto release the button.  */
    static const uint8_t LOCK_TIMER = 1500;               /**< The number of a milliseconds that the calibration and press lock is active.  Used to set active locks. */

    uint8_t pin;                /**< Analog signal pin of the IR sensor. */
    bool onceCalibrated;        /**< Flag to track the first time we calibrate. */
    bool isCalibrated;          /**< Flag to track if the eye is currently calibrated. */
    //bool calibBlink;          /**< Blink status during the first calibration. On/Off. */
    //uint32_t nextCalibBlink;  /**< Next time to change the first calibration blink status in millis. */
    uint32_t calibLock;         /**< Acts as a lock on the calibration process.  The number of milliseconds since the board started running must exceed this value before calibration can occur. */
    uint8_t calibCount;         /**< Number of time we read a calibration value within range in a row. */
    uint16_t irValue;           /**< Current IR sensor value. */
    uint16_t irMin;             /**< Minimum sensor value read. */
    uint16_t irMax;             /**< Maximum sensor value read. */
    uint16_t irThreshold;       /**< Current button press threshold. */
    uint16_t irLast;            /**< Last read sensor value. */
    uint32_t pressedLast;       /**< Time, in milliseconds, when we last detected a pressed state. */
    uint32_t pressedStart;      /**< Time, in milliseconds, when we detected the start of a press.  Used to trigger auto release of button */
    uint8_t pressedCount;       /**< Number of time we detected a pressed state consecutively.  If this value is greater than zero, the button is in a pressed state. */
    uint32_t pressDetectLock;   /**< Acts as a lock on detecting button presses. The number of milliseconds since the board started running must exceed this value before we detect button presses.  */

    #ifdef CLYDE_DEBUG_EYE
      uint16_t restartCount;    /**< Number of time calibration restarted because of noise since last calibration. */
    #endif   


/*
    // -- eye
    
    // this is so the brightness of the leds do not interfere with the ir sensor
    static const float SCALE_CONSTRAINT = 0.50;//0.88; 

     // sample every x milliseconds
    static const uint16_t TIME_THRESH = 150;

    // the size of our sample window, used to determine the
    // current_ir_val and press_thresh for the eye button
    static const uint16_t SAMPLE_SIZE = 10;

    // the threshold for ignoring raw data from the
    // difference between current and previous value
    static const uint16_t DELTA_THRESH = 80;

    // the threshold for throwing out data based on the
    // difference between the ir max and min
    static const uint16_t RANGE_THRESH = 40;

    // the number of presses to count to for is_pressed 
    // to be set to true
    static const uint8_t PRESS_COUNT_THRESH = 6;

    // larger: less sensitive, smaller: more sensitive
    static const uint16_t PRESS_SENSITIVITY = 300;

    // the amount of time (ms) from the last press to reset the
    // press count to 0 -- specifically when is_pressed is false
    static const uint16_t PRESS_LAST_RESET = 250;

    // if the amount of time from the start of the press to
    // now exceeds this amount, the press will be released
    static const uint16_t PRESS_START_TIMEOUT = 3000;

    // if the amount of time from the last press to now exceeds
    // this amount, then the press will be released
    static const uint16_t PRESS_LAST_TIMEOUT = 250;


    // -- eye / ir sample related
    long lastCalibrated;
    long lastSampled;
    uint16_t sampleCount;
    uint16_t irTotal;  //irSum
    uint16_t irRangeAvg;
    bool doneCalibration;


    // the raw, un-averaged, ir vals from analog in
    uint16_t currentIrRaw;
    uint16_t previousIrRaw;
    uint16_t irMinRaw;
    uint16_t irMaxRaw;


    // the averaged reading ir vals over time
    uint16_t currentIrAvg;  //currentIrVal
    uint16_t previousIrAvg;  //previousIrVal


    // eye button press related
    uint16_t pressThresh;
    long pressStart;
    bool isPressed;
    uint8_t pressSampleCount;
    long lastPress;

    */

public:  
  /** Constructor. */
  SquishyEye();
  
  /**
   * Initialize 
   */  
  bool init();

  /** Update the eye / infrared switch. */
  void update();

  bool isPressed();

  // -- callback methods
  void setPressedHandler(void(*function)()) { pressedHandler = function; }
  void setReleasedHandler(void(*function)()) { releasedHandler = function; }

  // -- callback methods
  void (*pressedHandler)();
  void (*releasedHandler)();
 
private:

  /**
   * Calibrate the eye.
   */
  void calibrate();

  void resetCalibration();

  void onPress();
  void onRelease();

  void autoRelease();

  
};


#endif
