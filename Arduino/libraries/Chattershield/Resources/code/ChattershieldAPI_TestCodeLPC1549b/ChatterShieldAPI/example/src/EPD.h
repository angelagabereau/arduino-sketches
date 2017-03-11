/*
 * EPD.h
 *
 *  Created on: Nov 23, 2014
 *      Author: FR
 */

#ifndef EPD_H_
#define EPD_H_

/* The displays are available in 1.44", 2" and 2.7" diagonal sizes with resolutions of 128x96, 200x96 and 264x176 pixels.
 * NOTE: Double buffering is necessary due to panel refresh process */

#define EPD_BufSmall		// uses minimal 1-bit per pixel screen buffers, instead of usual 2-bits per pixel;
							// "nothing" bits will be generated dynamically while refreshing the panel

#define EPD_1.44
//#define EPD_2.0
//#define EPD_2.7

#ifdef EPD_1.44
#define EPD_width		128
#define EPD_height		96
#else
#ifdef EPD_2.0
#define EPD_width		200
#define EPD_height		96
#else
#ifdef EPD_2.7
#define EPD_width		264
#define EPD_height		176
#endif	// EPD_2.7
#endif	// EPD_2.0
#endif	// EPD_1.44



#ifdef EPD_BufSmall
#define EPD_SizDiv		(8)		// 1 bit per pixel (1,536 Bytes for 1.44")
#else
#define EPD_SizDiv		(4)		// 2 bit per pixel (3,072 Bytes for 1.44")
#endif	// EPD_BufSmall


#define EPD_heightByt		(EPD_height/EPD_SizDiv)
#define EPD_widthByt		(EPD_width/EPD_SizDiv)
#define EPD_BufSizByt		((EPD_width*EPD_height)/EPD_SizDiv)






#endif /* EPD_H_ */
