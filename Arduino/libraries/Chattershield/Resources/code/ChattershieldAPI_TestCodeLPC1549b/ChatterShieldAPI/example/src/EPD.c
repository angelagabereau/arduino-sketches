/*
 * EPD.c
 *
 *  Created on: Nov 23, 2014
 *      Author: FR
 */

#include "lpc_types.h"
//#include "sys_config.h"
//#include "cmsis.h"
#include "EPD.h"

volatile   uint8_t EPDbuf1[EPD_heightByt][EPD_widthByt];	// NOTE: might be better to use uint32_t for efficiency on Cortex
volatile   uint8_t EPDbuf2[EPD_heightByt][EPD_widthByt];




