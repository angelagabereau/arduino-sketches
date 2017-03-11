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

#ifndef __COLORTYPES_H
#define __COLORTYPES_H

/* Used for color convwersion */
#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<math.h>

  /*The lower limit for H*/
#define HUE_LOWER_LIMIT (0.0)
/*The upper limit for H*/
#define HUE_UPPER_LIMIT (360.0)

  /*The lower limit for R, G, B (integer version)*/
#define RGBI_LOWER_LIMIT (0U)
/*The upper limit for R, G, B (integer version)*/
#define RGBI_UPPER_LIMIT (255U)


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "colorspace/colorspace.h"

/**
 * RGB color using integer values
 */
struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  
  RGB(uint8_t nr = 0, uint8_t ng = 0, uint8_t nb = 0) { r = nr; g = ng; b = nb; }
  
  RGB& operator =(const RGB& a)
  {
    r = a.r;
    g = a.g;
    b = a.b;
    return *this;
  }

};

/**
 * RGB color using float values
 */
struct RGBf {
  float r;
  float g;
  float b;
  
  RGBf(float nr = 0, float ng = 0, float nb = 0) { r = nr; g = ng; b = nb; }
  
  RGBf& operator =(const RGBf& a)
  {
    r = a.r;
    g = a.g;
    b = a.b;
    return *this;
  }
  
  RGBf& operator =(const RGB& a)
  {
    r = a.r;
    g = a.g;
    b = a.b;
    return *this;
  }

};


/**
 * HSV color
 */
struct HSI {
  float h;
  float s;
  float i;
  
  HSI(float nh = 0, float ns = 0, float ni = 0) { h = nh; s = ns; i = ni; }
  
  HSI& operator =(const HSI& a)
  {
    h = a.h;
    s = a.s;
    i = a.i;
    return *this;
  }
};

/**
 * Color Conversion Utlility
 * Convert between RGB and HSI color schemes.
 * https://github.com/dystopiancode/colorspace-conversions
 */
struct ColorConverter {


  RGB HSItoRGB(HSI hsi){

    Serial.println("HSItoRGB----");

    float r,g,b;

    Serial.println("HSI----");
    Serial.println(hsi.h);
    Serial.println(hsi.s);
    Serial.println(hsi.i);

/*
    Serial.println("RGB----");
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    return RGB(r,g,b);
*/

    //http://www.had2know.com/technology/hsi-rgb-color-converter-equations.html

    if (hsi.h == 0){
        // h = 0
        //R = I + 2IS
        //G = I - IS
        //B = I - IS.
        r = hsi.i + 2 * hsi.i * hsi.s;
        g = b = hsi.i - hsi.i * hsi.s;

    } else if (hsi.h > 0 && hsi.h < 120){
        // 0 < H < 120
        //R = I + IS*cos(H)/cos(60-H)
        //G = I + IS*[1 - cos(H)/cos(60-H)]
        //B = I - IS.
        r = hsi.i + hsi.i * hsi.s * cos(hsi.h)/cos(60-hsi.h);
        g = hsi.i + hsi.i * hsi.s * (1 - cos(hsi.h)/cos(60-hsi.h));
        b = hsi.i - hsi.i * hsi.s;
    } else if (hsi.h == 120){
        // h = 120
        //R = I - IS
        //G = I + 2IS
        //B = I - IS.
        r = b = hsi.i - hsi.i * hsi.s;
        g = hsi.i + 2 * hsi.i * hsi.s;

    } else if (hsi.h > 120 && hsi.h < 240){
        // 120 < h  < 240
        //R = I - IS
        //G = I + IS*cos(H-120)/cos(180-H)
        //B = I + IS*[1 - cos(H-120)/cos(180-H)]
        r = hsi.i - hsi.i * hsi.s;
        g = hsi.i + hsi.i * hsi.s * cos(hsi.h-120)/cos(180-hsi.h);
        b = hsi.i + hsi.i * hsi.s * (1 - cos(hsi.h-120)/cos(180-hsi.h));
    } else if (hsi.h == 240){
        // h=240
        //R = I - IS
        //G = I - IS
        //B = I + 2IS.
        r = g = hsi.i - hsi.i * hsi.s;
        b = hsi.i + 2 * hsi.i * hsi.s;
    } else if (hsi.h > 240 && hsi.h < 360){
        // 240 < h < 360 
        //R = I + IS*[1 - cos(H-240)/cos(300-H)]
        //G = I - IS
        //B = I + IS*cos(H-240)/cos(300-H)
        r = hsi.i + hsi.i * hsi.s * (1 - cos(hsi.h-240)/cos(300-hsi.h));
        g = hsi.i - hsi.i * hsi.s;
        b = hsi.i + hsi.i * hsi.s * cos(hsi.h-240)/cos(300-hsi.h);
    }

/*
        if (hsi.h >= 0.0 && hsi.h <= (HUE_UPPER_LIMIT / 3.0))
        {
            b = (1.0 / 3.0) * (1.0 - hsi.s);
            r = (1.0 / 3.0) * ((hsi.s * cos(hsi.h)) / cos(60.0 - hsi.h));
            g = 1.0 - (b + r);
        }
        else if (hsi.h > (HUE_UPPER_LIMIT / 3.0)
                && hsi.h <= (2.0 * HUE_UPPER_LIMIT / 3.0))
        { Serial.println("Middle");
            hsi.h -= (HUE_UPPER_LIMIT / 3.0);
            r = (1.0 / 3.0) * (1.0 - hsi.s);
            g = (1.0 / 3.0) * ((hsi.s * cos(hsi.h)) / cos(60.0 - hsi.h));
            b = 1.0 - (g + r);

        }
        else //h>240 h<360 
        {
            hsi.h -= (2.0 * HUE_UPPER_LIMIT / 3.0);
            g = (1.0 / 3.0) * (1.0 - hsi.s);
            b = (1.0 / 3.0) * ((hsi.s * cos(hsi.h)) / cos(60.0 - hsi.h));
            r = 1.0 - (g + b);
        }

*/
    Serial.println("RGB----");
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    Serial.println("again RGB----");
            r = (uint8_t) (r * (double) RGBI_UPPER_LIMIT + 0.5);
            g = (uint8_t) (g * (double) RGBI_UPPER_LIMIT + 0.5);
            b = (uint8_t) (b * (double) RGBI_UPPER_LIMIT + 0.5);

    Serial.println(r);
    Serial.println(g);
    Serial.println(b);

   // return RGB(r,g,b);


  }

  HSI RGBtoHSI(RGB rgb) {

    Serial.println("RGBtoHSI----");

    float h,s,i;
    float r = (float)rgb.r;
    float g = (float)rgb.g;
    float b = (float)rgb.b;



    Serial.println("RGB----");

    float m = getMinimum(r, g, b);
    float M = getMaximum(r, g, b);

    Serial.println("min----");
    Serial.println(m);
    Serial.println("max----");
    Serial.println(M);


    i = (1.0 / 3.0) * (r + g + b);

    if(i=0){
      s=0;
    }else{
      s = 1.0 - (m / i);
    }
/*
    if(g >= b){
        // G ≥ B, 
        // H = cos-1[ (R - ½G - ½B)/√R² + G² + B² - RG - RB - GB ] 
      h = acos( (r - g*0.5 - b*0.5) / sqrt(r*r + g*g² + b*b - r*g - r*b - g*b ));

    }else{
        // B > G
        // H = 360 - cos-1[ (R - ½G - ½B)/√R² + G² + B² - RG - RB - GB ]
     // h = 360 - acos( (r - g*0.5 - b*0.5) / sqrt(r*r + g*g² + b*b - r*g - r*b - g*b ));
    }
*/

 /*   float c = M - m;

    i = (1.0 / 3.0) * (r + g + b);
    if (c == 0)
    {
        h = 0.0;
        s = 0.0;
    }
    else
    {
        if (M == r)
        {
            h = fmod(((g - b) / c), 6.0);
        }
        else if (M == g)
        {
            h = (b - r) / c + 2.0;
        }
        else if (M == b)
        {
            h = (r - g) / c + 4.0;
        }
        h *= 60.0;
        s = 1.0 - (m / i);
    }
*/
    Serial.println("HSI----");
    Serial.println(h);
    Serial.println(s);
    Serial.println(i);

    return HSI(h, s, i);

  };

  float getMinimum(float r, float g, float b)
  {
      if (r < g)
      {
          if (r < b)
          {
              return r;
          }
          else
          {
              return b;
          }
      }
      else
      {
          if (g < b)
          {
              return g;
          }
          else
          {
              return b;
          }
      }
      return 0;
  }

  float getMaximum(float r, float g, float b)
  {
      if (r > g)
      {
          if (r > b)
          {
              return r;
          }
          else
          {
              return b;
          }
      }
      else
      {
          if (g > b)
          {
              return g;
          }
          else
          {
              return b;
          }
      }
      return 0;
  }

};

#endif
