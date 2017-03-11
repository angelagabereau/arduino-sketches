/*
  vs1003_hello
   
  A simple MP3 flow player to test if board and library are working together.
  The following sketch should say "Hello" every 0.5s :)
  Created 2012-04-05 by Andrey Karpov
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>

#include <Arduino.h>

#include "printf.h"
#include <VS1003.h>
#include <SPI.h>

/*
 * VS1003 development board connected by it's header pins the following way:
 *
 * GND  - GND
 * XDCS - D6
 * DREQ - D7
 * XRES - D8
 * XCS  - D9
 * SCLK - D13
 * SI   - D11
 * SO   - D12
 * GND  - GND
 * 5V   - 5V
 */
VS1003 player(9, 6, 7, 8); // cs_pin, dcs_pin, dreq_pin, reset_pin

unsigned char HelloMP3[] = {
  0xFF,0xF2,0x40,0xC0,0x19,0xB7,0x00,0x14,0x02,0xE6,0x5C, /* ..@.......\ */
  0x01,0x92,0x68,0x01,0xF1,0x5E,0x03,0x08,0xF0,0x24,0x80, /* ..h..^...$. */
  0x05,0x9E,0x20,0xC6,0xFC,0x12,0x32,0x5C,0xBF,0xF9,0xB9, /* .. ...2\... */
  0x20,0x4A,0x7F,0x85,0xEC,0x4C,0xCD,0xC7,0x27,0xFE,0x5C, /*  J...L..'.\ */
  0x34,0x25,0xCB,0xE6,0xFF,0xFF,0x8E,0x42,0xE1,0xA0,0x5E, /* 4%.....B..^ */
  0xCA,0x6E,0x30,0x9F,0xFF,0xF8,0xC2,0x12,0x84,0xB9,0x7C, /* .n0.......| */
  0xDC,0x61,0x09,0x4A,0x7F,0xFF,0xFF,0xF9,0x7D,0x32,0x51, /* .a.J....}2Q */
  0x09,0x7C,0xE1,0xA5,0x6E,0xB4,0xFF,0xFF,0xFF,0xFF,0xD3, /* .|..n...... */
  0x34,0x41,0x91,0xF0,0x11,0x8F,0x00,0x0F,0x81,0x9C,0x10, /* 4A......... */
  0xEE,0x59,0xCE,0x56,0x67,0xFF,0xF2,0x42,0xC0,0xEC,0x53, /* .Y.Vg..B..S */
  0x09,0x15,0xF9,0xAA,0xA8,0x0D,0xD9,0x40,0x00,0xCA,0x34, /* .......@..4 */
  0x53,0xD9,0x18,0xAB,0x7D,0xF7,0x89,0x3F,0x11,0x38,0x94, /* S...}..?.8. */
  0x82,0x59,0x93,0x20,0x6A,0x0C,0xEE,0x8E,0x58,0xFA,0x38, /* .Y. j...X.8 */
  0x82,0xCA,0xF0,0x58,0xBB,0xDA,0x0C,0x50,0x56,0x1F,0xBB, /* ...X...PV.. */
  0x18,0x5D,0x8B,0x9F,0xDA,0x71,0x4F,0xFF,0xBD,0xFE,0xEF, /* .]...qO.... */
  0x69,0x36,0x86,0x3C,0x50,0xBB,0x0A,0x07,0x89,0x54,0xF0, /* i6.<P....T. */
  0x88,0x9F,0x90,0x95,0x30,0x94,0x2E,0x7E,0xF0,0x64,0x96, /* ....0..~.d. */
  0x79,0x08,0x3E,0x20,0x97,0x28,0x34,0x9C,0x09,0x7F,0xD2, /* y.> .(4.... */
  0xC0,0x01,0x75,0xF8,0x05,0x6B,0x5F,0x41,0x17,0x0B,0xE7, /* ..u..k_A... */
  0xFF,0xF2,0x40,0xC0,0x61,0xE5,0x0B,0x16,0x09,0xC6,0xC5, /* ..@.a...... */
  0x74,0x7B,0xCC,0x94,0x7A,0xF7,0x80,0x76,0xB2,0xD2,0xF8, /* t{..z..v... */
  0x39,0x06,0x38,0xFD,0x71,0xC5,0xDE,0x3A,0x38,0xBF,0xD5, /* 9.8.q..:8.. */
  0xF7,0x12,0x37,0xCB,0xF5,0x63,0x0C,0x9B,0xCE,0x77,0x25, /* ..7..c...w% */
  0xED,0xFB,0x3D,0x6B,0x35,0xF9,0x6D,0xD7,0xF9,0x2C,0xD1, /* ..=k5.m..,. */
  0x97,0x15,0x87,0x93,0xA4,0x49,0x4A,0x18,0x16,0x07,0xA1, /* .....IJ.... */
  0x60,0xF7,0x52,0x94,0xDB,0x02,0x16,0x70,0xB2,0xD8,0x80, /* `.R....p... */
  0x30,0xC2,0x94,0x40,0x81,0x74,0x5A,0x19,0x7A,0x80,0x60, /* 0..@.tZ.z.` */
  0x41,0x21,0x46,0x95,0xD5,0xC4,0x40,0xD2,0x01,0xC0,0x01, /* A!F...@.... */
  0xDA,0xD9,0xA0,0xB1,0x01,0xFF,0xF2,0x42,0xC0,0x82,0x10, /* .......B... */
  0x0B,0x12,0xF9,0x9E,0xC9,0x7E,0x7A,0xC6,0x95,0x55,0x09, /* .....~z..U. */
  0x8B,0x19,0x5E,0x8B,0x26,0xCA,0xEB,0x68,0x8A,0x05,0x8F, /* ..^.&..h... */
  0x36,0xA5,0xA5,0x03,0xB8,0x9C,0xED,0x24,0x51,0x59,0x90, /* 6......$QY. */
  0xF6,0xC5,0x7D,0xB5,0xAD,0xAF,0xF6,0x3B,0x18,0xEF,0x3F, /* ..}....;..? */
  0xFF,0xFF,0x4E,0xDE,0x16,0x66,0x0B,0xAA,0x33,0x23,0xDD, /* ..N..f..3#. */
  0x9C,0x4E,0x6E,0x55,0x22,0x9D,0xA2,0x40,0xA6,0x36,0x31, /* .NnU"..@.61 */
  0x69,0xA5,0xE1,0xD9,0x7F,0xF7,0xC6,0xCC,0x48,0x00,0x0E, /* i.......H.. */
  0x90,0x16,0x00,0x0F,0xDE,0x6E,0x80,0x11,0x0C,0x9A,0x4F, /* .....n....O */
  0x56,0xDB,0x88,0xD3,0xB2,0x1C,0x00,0xE0,0x2E,0x3E,0xAC, /* V........>. */
  0xFF,0xF2,0x40,0xC0,0x1C,0xE5,0x19,0x13,0x31,0x4E,0xCD, /* ..@.....1N. */
  0x9E,0xC3,0x06,0x71,0x03,0x85,0xE5,0xB5,0x6D,0x88,0x50, /* ...q....m.P */
  0x8E,0x0E,0x17,0x3B,0x19,0xFB,0x4E,0x3B,0x99,0xEF,0x4C, /* ...;..N;..L */
  0x9E,0xF7,0x7B,0x31,0x7C,0x3C,0x5F,0xFF,0xF4,0xF8,0xE3, /* ..{1|<_.... */
  0x92,0x42,0x07,0x8E,0x83,0x8E,0x0F,0x05,0x08,0x91,0xA3, /* .B......... */
  0x16,0xE2,0xDF,0xB7,0x62,0x60,0x48,0x31,0x3C,0xFF,0xD4, /* ....b`H1<.. */
  0x9E,0x0C,0x68,0x00,0x77,0x54,0xE3,0x1E,0x05,0xC5,0xF8, /* ..h.wT..... */
  0xEA,0x8D,0x82,0x9D,0x08,0xA9,0x06,0x8D,0x1E,0x5D,0x7C, /* .........]| */
  0x7F,0x08,0xC0,0x50,0x45,0x42,0xD0,0x36,0xF8,0xB2,0x4D, /* ...PEB.6..M */
  0x53,0x0C,0x80,0x3B,0x4D,0xFF,0xF2,0x42,0xC0,0x2F,0x3C, /* S..;M..B./< */
  0x25,0x19,0x29,0xFE,0xBC,0x2E,0xC4,0xD0,0x99,0x4C,0x48, /* %.)......LH */
  0xB0,0x9C,0x49,0xD2,0x1A,0x2D,0x02,0xC2,0x79,0x69,0x16, /* ..I..-..yi. */
  0x92,0xA8,0xC5,0xAB,0x45,0x5A,0x68,0xE8,0x75,0x57,0xCD, /* ....EZh.uW. */
  0xF1,0xB9,0xAA,0x13,0x88,0xE4,0x87,0x42,0x15,0xB3,0x58, /* .......B..X */
  0xF5,0xA3,0x46,0xB1,0xCF,0xD3,0x59,0x7E,0xBA,0xB5,0xA7, /* ..F...Y~... */
  0x6B,0x0B,0x17,0x57,0x6B,0x5C,0x4A,0xCD,0x53,0x76,0x2A, /* k..Wk\J.Sv* */
  0x1D,0x28,0xC5,0x1C,0x76,0x5C,0xDD,0x0A,0x00,0x4B,0xC0, /* .(..v\...K. */
  0x1B,0xCA,0xA8,0xE9,0x81,0x5B,0xA6,0xDC,0xA4,0x59,0x13, /* .....[...Y. */
  0xFC,0xBA,0x8F,0x98,0x79,0x44,0x25,0xC9,0x35,0x38,0xCA, /* ....yD%.58. */
  0xFF,0xF2,0x40,0xC0,0xB9,0x7D,0x1A,0x13,0x79,0x6A,0xC8, /* ..@..}..yj. */
  0x3E,0xC4,0x46,0x94,0x8D,0x3C,0x67,0x85,0xB1,0xA8,0x89, /* >.F..<g.... */
  0xC0,0xF2,0xE6,0x2F,0x9D,0x7C,0xC9,0xB4,0xBE,0xCF,0xE1, /* .../.|..... */
  0x7D,0xFE,0x1F,0x03,0x00,0x12,0x84,0x72,0x8C,0xE7,0xD8, /* }......r... */
  0x5E,0xC9,0xA9,0x01,0xBA,0x9B,0xC4,0x10,0x5C,0x70,0x2E, /* ^.......\p. */
  0x6C,0x48,0xE7,0x8C,0x15,0x0B,0x06,0x01,0xE5,0xFF,0xFF, /* lH......... */
  0xD4,0x0D,0x00,0x0F,0xCE,0x58,0x95,0x61,0xA8,0x9E,0x7B, /* .....X.a..{ */
  0x19,0x98,0xB0,0xF0,0xC6,0x72,0x82,0xD5,0x27,0x06,0x47, /* .....r..'.G */
  0x41,0x22,0x0F,0x65,0x93,0xC9,0x8A,0x09,0x19,0x48,0x1B, /* A".e.....H. */
  0xBD,0xD6,0x64,0x1A,0xAC,0xFF,0xF2,0x42,0xC0,0xF1,0x11, /* ..d....B... */
  0x25,0x14,0x22,0x06,0xBC,0x0E,0xD4,0x4E,0x99,0x90,0xA8, /* %."....N... */
  0xD8,0xB7,0xAD,0x5D,0x3E,0xAF,0x6E,0xBE,0x66,0x83,0xA4, /* ...]>.n.f.. */
  0xE3,0xC2,0xE0,0x29,0x43,0x87,0x5F,0x4F,0x27,0x9C,0x2C, /* ...)C._O'., */
  0xD0,0x91,0xF3,0x87,0x9B,0x54,0xED,0xD1,0xB4,0xF3,0x39, /* .....T....9 */
  0x87,0x22,0x06,0x86,0x0D,0x71,0xE4,0x6F,0x2A,0x08,0x04, /* ."...q.o*.. */
  0xC0,0x03,0x2A,0xB1,0xE2,0x05,0x4D,0x64,0xA1,0x9C,0xA6, /* ..*...Md... */
  0x0D,0x41,0xA6,0xF2,0x7A,0xC1,0x30,0xC3,0x38,0x26,0x09, /* .A..z.0.8&. */
  0x50,0x08,0xC4,0xF6,0x30,0x0C,0xA6,0xA9,0x17,0x00,0x13, /* P...0...... */
  0x0C,0xDC,0xC4,0x2F,0x28,0xEB,0x3F,0xCD,0x7A,0x3D,0x2F, /* .../(.?.z=/ */
  0xFF,0xF2,0x40,0xC0,0x18,0x6F,0x2E,0x13,0xA1,0xF2,0xBC, /* ..@..o..... */
  0x36,0xCB,0x4E,0x99,0x6E,0xFC,0xEE,0xC5,0xF0,0xA0,0xB7, /* 6.N.n...... */
  0x92,0xD4,0xEE,0x79,0x7C,0x50,0x5D,0xE5,0x04,0x94,0xA9, /* ...y|P].... */
  0x76,0xCF,0x6C,0x70,0xDD,0x0D,0xD4,0xEE,0xED,0x98,0xE8, /* v.lp....... */
  0xC8,0x35,0x36,0x7A,0x0C,0x05,0x80,0x03,0xBC,0xBE,0x91, /* .56z....... */
  0x00,0x7C,0xAE,0x65,0xB8,0x91,0xA3,0x33,0xBA,0x68,0x60, /* .|.e...3.h` */
  0xD4,0x1A,0x66,0xF8,0x43,0xA0,0x20,0x89,0xE7,0x80,0xD8, /* ..f.C. .... */
  0x1E,0x4F,0xA0,0x04,0x60,0x06,0x0A,0xA4,0x91,0x24,0xFA, /* .O..`....$. */
  0x9F,0x57,0x53,0xF4,0x7A,0xDB,0x5F,0x56,0xE3,0x6E,0x0B, /* .WS.z._V.n. */
  0x8B,0x3A,0x1C,0xF9,0x5E,0xFF,0xF2,0x42,0xC0,0xB1,0x00, /* .:..^..B... */
  0x38,0x14,0x09,0xEE,0xB4,0x36,0xD3,0x4E,0x99,0xA4,0x78, /* 8....6.N..x */
  0x94,0x73,0xC4,0x66,0x30,0xF5,0xEA,0xDB,0xBA,0x67,0x67, /* .s.f0....gg */
  0x95,0x6B,0xAB,0x68,0x5D,0x08,0xA1,0x39,0x56,0xAB,0x1E, /* .k.h]..9V.. */
  0xD5,0x03,0xE8,0x01,0x70,0x00,0xB3,0x93,0x33,0x19,0x8C, /* ....p...3.. */
  0x61,0x8F,0xBB,0x5D,0x24,0x12,0x63,0xD3,0x4B,0x5D,0x91, /* a..]$.c.K]. */
  0x08,0x43,0x22,0x56,0x1A,0xC5,0x10,0x21,0x84,0xA8,0xEA, /* .C"V...!... */
  0x80,0xBF,0x16,0x8E,0x3D,0x46,0x18,0x9C,0x6E,0x9A,0x91, /* ....=F..n.. */
  0xE6,0xC9,0x6F,0xD2,0x7D,0x27,0xD7,0xE9,0x6B,0xFF,0x0A, /* ..o.}'..k.. */
  0x03,0x43,0x89,0xD5,0xBF,0x52,0x97,0x0A,0x25,0x95,0x0D, /* .C...R..%.. */
  0xFF,0xF2,0x40,0xC0,0xF5,0xC3,0x41,0x13,0x81,0xEE,0xA8, /* ..@...A.... */
  0x5E,0xD3,0x44,0x98,0xFC,0xCF,0x97,0xF9,0x58,0xB5,0x33, /* ^.D.....X.3 */
  0xB1,0x85,0x47,0x86,0xD7,0x98,0x01,0x3B,0xA3,0x4F,0x7E, /* ..G....;.O~ */
  0x04,0xA6,0xC3,0x39,0x21,0x70,0x27,0x62,0xB5,0x18,0x10, /* ...9!p'b... */
  0x09,0x99,0x00,0x8B,0x7E,0xF2,0xBF,0x52,0x18,0x26,0x30, /* ....~..R.&0 */
  0x1C,0xB0,0x01,0x49,0x30,0xE0,0xC3,0x11,0x46,0x05,0xCC, /* ...I0...F.. */
  0x49,0x14,0x28,0xB2,0xED,0x4B,0x57,0x5A,0x2F,0xB7,0x46, /* I.(..KWZ/.F */
  0x63,0x34,0xD2,0xDA,0x9F,0x56,0x32,0xB7,0xA2,0x25,0xFF, /* c4...V2..%. */
  0x94,0x28,0x33,0x7F,0x3B,0xC4,0x50,0xEC,0xB1,0xE2,0x26, /* .(3.;.P...& */
  0xA1,0xB7,0x07,0x7F,0xFB,0xFF,0xF2,0x42,0xC0,0x67,0x6A, /* .......B.gj */
  0x4C,0x13,0xF9,0x6A,0x90,0x7E,0xDB,0x44,0x94,0x3F,0xFF, /* L..j.~.D.?. */
  0x14,0xD6,0x2A,0xFF,0xFF,0xC1,0x34,0x8C,0x48,0x22,0x00, /* ..*...4.H". */
  0x06,0x8F,0x21,0xFD,0x64,0x60,0x04,0x92,0x42,0xEA,0x74, /* ..!.d`..B.t */
  0x32,0x37,0xAA,0x5A,0x9F,0x67,0x01,0x8B,0x3F,0x37,0x31, /* 27.Z.g..?71 */
  0xDD,0x06,0x3C,0x01,0x34,0x30,0xE0,0x5C,0x78,0x78,0xCB, /* ..<.40.\xx. */
  0xD6,0xF1,0x31,0x8A,0x69,0x61,0x93,0x92,0x42,0xCE,0x4B, /* ..1.ia..B.K */
  0xC5,0x02,0x4E,0x73,0xC6,0x24,0x30,0xCD,0x08,0x66,0xC6, /* ..Ns.$0..f. */
  0x35,0xAB,0xA2,0x3D,0x2F,0xB3,0xBD,0x34,0x87,0x13,0xEE, /* 5..=/..4... */
  0x71,0x45,0x68,0xFA,0xEA,0x05,0x84,0x41,0x36,0x4C,0x9A, /* qEh....A6L. */
  0xFF,0xF2,0x40,0xC0,0xC9,0x92,0x56,0x13,0xD0,0x6E,0x70, /* ..@...V..np */
  0x54,0xD3,0xCC,0x28,0x06,0xD7,0x0E,0xA4,0x1D,0x9C,0x9D, /* T..(....... */
  0xD9,0xA9,0x88,0x7B,0xB5,0xA3,0x56,0xB7,0x4B,0x4B,0x5A, /* ...{..V.KKZ */
  0x9B,0x2C,0xA9,0xAD,0x6F,0x99,0x6C,0xC0,0x4C,0x14,0x14, /* .,..o.l.L.. */
  0xEF,0xB4,0x20,0x91,0x5F,0xBC,0x81,0x41,0x41,0x5D,0xD4, /* .. ._..AA]. */
  0x20,0xBD,0x05,0x1A,0x6F,0xE2,0x68,0x56,0x41,0x41,0x57, /*  ...o.hVAAW */
  0xF9,0xBF,0x89,0x82,0x8E,0xC7,0x8F,0x0A,0x0A,0x09,0x37, /* ..........7 */
  0xF1,0x05,0x0A,0x0A,0x0A,0x0A,0x09,0x05,0x37,0xFF,0x10, /* ........7.. */
  0x50,0x50,0x53,0x65,0xFF,0xFF,0xFD,0x75,0xDF,0xFF,0xFF, /* PPSe...u... */
  0x68,0x4F,0xFF,0x84,0x70,0xFF,0xF2,0x42,0xC0,0x27,0x50, /* hO..p..B.'P */
  0x5F,0x17,0xE8,0x82,0x3C,0x11,0x58,0x18,0x01,0x55,0x48, /* _...<.X..UH */
  0xBC,0x52,0xFC,0x4A,0x4C,0x3C,0xD5,0xF6,0x11,0x2D,0xBF, /* .R.JL<...-. */
  0xEA,0x03,0x5C,0x57,0x29,0xBF,0xC3,0x75,0x1C,0xE6,0xDD, /* ..\W)..u... */
  0xBF,0xED,0xEF,0xD0,0x98,0x77,0x71,0x95,0x73,0xFF,0xED, /* .....wq.s.. */
  0x54,0xBE,0xD5,0xEE,0xAE,0xC2,0xD5,0x0B,0xFF,0xF1,0x97, /* T.......... */
  0x8A,0xE4,0x42,0x09,0x99,0xB1,0xEA,0x94,0xDC,0x78,0xB5, /* ..B......x. */
  0x34,0x0F,0xF1,0x8F,0xFC,0x15,0xF6,0xFA,0xB1,0x47,0xA9, /* 4........G. */
  0x6C,0x67,0x43,0x8B,0xF2,0x76,0x22,0xED,0xDA,0x85,0xBA, /* lgC..v".... */
  0x2F,0xC7,0xF9,0xCF,0xFC,0xDB,0x46,0x2E,0x50,0x0A,0x84, /* /.....F.P.. */
  0xFF,0xF2,0x40,0xC0,0xC6,0x4A,0x59,0x28,0x2B,0x19,0xE0, /* ..@..JY(+.. */
  0x01,0x89,0x78,0x00,0x52,0x85,0x3C,0x8E,0x54,0x9A,0x48, /* ..x.R.<.T.H */
  0x5A,0x72,0x32,0x94,0xBF,0x43,0x4F,0x24,0x53,0x4B,0xEC, /* Zr2..CO$SK. */
  0x4B,0x99,0x0E,0x66,0x1F,0xFF,0xCE,0x7F,0xFF,0x3F,0x10, /* K..f.....?. */
  0xAE,0x82,0x62,0x71,0x34,0x18,0x59,0x9B,0x51,0xC7,0x59, /* ..bq4.Y.Q.Y */
  0xCE,0xEE,0xA5,0xFE,0x02,0xBB,0x30,0x91,0x49,0xD5,0x4B, /* ......0.I.K */
  0xF3,0xDC,0x9A,0xA9,0x57,0x8E,0x72,0x10,0xC0,0x5D,0x60, /* ....W.r..]` */
  0x67,0xFC,0x7D,0xD6,0xBA,0xDD,0xB3,0x8B,0x5A,0x0A,0x4C, /* g.}.....Z.L */
  0x41,0x4D,0x45,0x33,0x2E,0x39,0x33,0xAA,0xAA,0xAA,0xAA, /* AME3.93.... */
  0xAA,0xAA,0xAA,0xAA,0xAA,0x54,0x41,0x47,0x48,0x65,0x6C, /* .....TAGHel */
  0x6C,0x6F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, /* lo          */
  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, /*             */
  0x20,0x20,0x20,0x20,0x20,0x50,0x61,0x6E,0x75,0x2D,0x4B, /*      Panu-K */
  0x72,0x69,0x73,0x74,0x69,0x61,0x6E,0x20,0x50,0x6F,0x69, /* ristian Poi */
  0x6B,0x73,0x61,0x6C,0x6F,0x20,0x20,0x20,0x20,0x20,0x20, /* ksalo       */
  0x20,0x20,0x56,0x53,0x44,0x53,0x50,0x20,0x54,0x65,0x73, /*   VSDSP Tes */
  0x74,0x69,0x6E,0x67,0x20,0x20,0x20,0x20,0x20,0x20,0x20, /* ting        */
  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, /*             */
  0x20,0x20,0x20,0x4D,0x50,0x33,0x20,0x48,0x65,0x6C,0x6C, /*    MP3 Hell */
  0x6F,0x2C,0x20,0x57,0x6F,0x72,0x6C,0x64,0x21,0x20,0x20, /* o, World!   */
  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, /*             */
  0x00, /* . */
};

void setup () {
	// initiate SPI
    SPI.begin();
    // initiate a serial port at 57600
   	Serial.begin(57600);
    // internal routines                                                                                                                                         
  	printf_begin();                                                                                                                                                            
  	printf_P(PSTR(__FILE__ "\r\n"));                                                                                                                                           
	// initiate a player
    player.begin();
	// set maximum output volume
    player.setVolume(0x00);
}

void loop() {
    // play hellomp3 flow each 0.5s ;)
    player.playChunk(HelloMP3, sizeof(HelloMP3));
    delay(500);
}
