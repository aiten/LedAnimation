/*
This file is part of CNCLib - A library for stepper motors.

Copyright(c) Herbert Aitenbichler

Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated documentation files(the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <Arduino.h>
#include "GFXExtension.h"

#if defined(__SAM3X8E__)

#define pgm_read_ptr pgm_read_dword

#else

//#define pgm_read_ptr pgm_read_word

#endif

uint8_t GFXExtension::_charSizeMult = 0;

/******************************************************************************/

const uint8_t* GFXExtension::ToAsciiArray(char ch, uint8_t& charXsize, int8_t& shift)
{
	const uint8_t* out    = ToAsciiArray(ch);
	const uint8_t* outend = out;
	charXsize             = 0;
	shift                 = 0;

	for (charXsize = 0; ((pgm_read_byte(outend)) & 0x80) == 0;)
	{
		outend++;
		charXsize++;
	}

	uint8_t shiftValue = pgm_read_byte(outend) & 3;
	uint8_t shiftSign  = pgm_read_byte(outend) & 4;

	shift = shiftSign ? -4 + shiftValue : shiftValue;

	return out;
}

/******************************************************************************/

const uint8_t* GFXExtension::ToAsciiArray(char ch)
{
	//	Serial.print(ch);
	//	Serial.println((int)ch);

	switch (ch)
	{
	case '-': return __Minus;
	case '+': return __Plus;
	case '*': return __Mult;
	case '.': return __Dot;
	case '!': return __Exp;
	case '/': return __Div;
	case ',': return __Comma;
	case '?': return __Qest;
	case '(': return __BracketO;
	case ')': return __BracketC;
	case '\344': return __ae;
	case '\304': return __Ae;
	case '\366': return __oe;
	case '\326': return __Oe;
	case '\374': return __ue;
	case '\334': return __Ue;
	case '\337': return __Sz;
	//	case '?':
	}

	if (ch >= 'A' && ch <= 'Z')
	{
		return (const uint8_t*)pgm_read_ptr(&__ABC[ch - 'A']);
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		return (const uint8_t*)pgm_read_ptr(&__abc[ch - 'a']);
	}
	else if (ch >= '0' && ch <= '9')
	{
		return (const uint8_t*)pgm_read_ptr(&__0123[ch - '0']);
	}

	return __Blank;
}

/******************************************************************************/

uint8_t GFXExtension::ScrollInRight(char ch, uint16_t delayTime, CAskContinue* askContinue, uint16_t col, uint16_t fillcol)
{
	uint8_t        charXsize;
	int8_t         shift;
	uint8_t        i;
	const uint8_t* out = ToAsciiArray(ch, charXsize, shift);
	shift *= -_charSizeMult;
	shift += ((GFX()->height() - (CHARHEIGHT + 1) * _charSizeMult)) / 2;

	for (i = 0; i < charXsize; i++, out++)
	{
		for (uint8_t n = 0; n < _charSizeMult; n++)
		{
			ScrollLeft(fillcol);

			uint8_t mask = 1;
			// for (uint16_t y = CHARHEIGHT-1; y != 0xff; y--)
			for (uint16_t y = 1; y < CHARHEIGHT; y++)
			{
				for (uint8_t m = 0; m < _charSizeMult; m++)
				{
					GFX()->drawPixel(GFX()->width() - 1, m + y * _charSizeMult + shift, (pgm_read_byte(out) & mask) != 0 ? col : fillcol);
				}
				mask *= 2;
			}
			askContinue->Delay(delayTime);
		}
	}
	return charXsize * _charSizeMult;
}

/******************************************************************************/

const uint16_t GFXExtension::NoColor         = 0xffff;
const uint16_t GFXExtension::OffColor        = 0;
const uint8_t  GFXExtension::NoPallet16Color = 0xff;

/******************************************************************************/

const uint8_t GFXExtension::__Blank[] PROGMEM =
{
	0b00000000,
	0b10000000
};

const uint8_t GFXExtension::__Dot[] PROGMEM =
{
	0b01000000,
	0b10000000
};

const uint8_t GFXExtension::__Comma[] PROGMEM =
{
	0b01000000,
	0b00110000,
	0b10000001
};

const uint8_t GFXExtension::__Minus[] PROGMEM =
{
	0b00001000,
	0b00001000,
	0b00001000,
	0b10000000
};

const uint8_t GFXExtension::__Plus[] PROGMEM =
{
	0b00001000,
	0b00011100,
	0b00001000,
	0b10000000
};

const uint8_t GFXExtension::__Mult[] PROGMEM =
{
	0b00101010,
	0b00011100,
	0b00011100,
	0b00101010,
	0b10000000
};

const uint8_t GFXExtension::__Div[] PROGMEM =
{
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b10000000
};

const uint8_t GFXExtension::__Exp[] PROGMEM =
{
	0b01011111,
	0b10000000
};
const uint8_t GFXExtension::__BracketO[] PROGMEM =
{
	0b00111110,
	0b01000001,
	0b10000000
};
const uint8_t GFXExtension::__BracketC[] PROGMEM =
{
	0b01000001,
	0b00111110,
	0b10000000
};
const uint8_t GFXExtension::__Qest[] PROGMEM =
{
	0b00000010,
	0b01011001,
	0b00000110,
	0b00000000,
	0b10000000
};
const uint8_t GFXExtension::__Sz[] PROGMEM =
{
	0b01111110,
	0b00001001,
	0b01110100,
	0b00000000,
	0b10000000
};


const uint8_t GFXExtension::__A[] PROGMEM =
{
	0b01111100,
	0b00010010,
	0b00010001,
	0b00010010,
	0b01111100,
	0b10000000
};
const uint8_t GFXExtension::__Ae[] PROGMEM =
{
	0b01111101,
	0b00010010,
	0b00010001,
	0b00010010,
	0b01111101,
	0b10000000
};
const uint8_t GFXExtension::__B[] PROGMEM =
{
	0b01111111,
	0b01001001,
	0b01001001,
	0b00110110,
	0b10000000
};
const uint8_t GFXExtension::__C[] PROGMEM =
{
	0b00111110,
	0b01000001,
	0b01000001,
	0b00100010,
	0b10000000
};
const uint8_t GFXExtension::__D[] PROGMEM =
{
	0b01111111,
	0b01000001,
	0b01000001,
	0b00111110,
	0b10000000
};
const uint8_t GFXExtension::__E[] PROGMEM =
{
	0b01111111,
	0b01001001,
	0b01001001,
	0b01001001,
	0b10000000
};
const uint8_t GFXExtension::__F[] PROGMEM =
{
	0b01111111,
	0b00001001,
	0b00001001,
	0b00001001,
	0b10000000
};
const uint8_t GFXExtension::__G[] PROGMEM =
{
	0b00111110,
	0b01000001,
	0b01001001,
	0b00111010,
	0b10000000
};
const uint8_t GFXExtension::__H[] PROGMEM =
{
	0b01111111,
	0b00001000,
	0b00001000,
	0b01111111,
	0b10000000
};
const uint8_t GFXExtension::__I_[] PROGMEM =
{
	0b01000001,
	0b01111111,
	0b01000001,
	0b10000000
};
const uint8_t GFXExtension::__J[] PROGMEM =
{
	0b00100000,
	0b01000000,
	0b01000000,
	0b00111111,
	0b10000000
};
const uint8_t GFXExtension::__K[] PROGMEM =
{
	0b01111111,
	0b00010100,
	0b00100010,
	0b01000001,
	0b10000000
};
const uint8_t GFXExtension::__L[] PROGMEM =
{
	0b01111111,
	0b01000000,
	0b01000000,
	0b01000000,
	0b10000000
};
const uint8_t GFXExtension::__M[] PROGMEM =
{
	0b01111111,
	0b00000010,
	0b00000100,
	0b00000010,
	0b01111111,
	0b10000000
};
const uint8_t GFXExtension::__N[] PROGMEM =
{
	0b01111111,
	0b00000010,
	0b00001000,
	0b00100000,
	0b01111111,
	0b10000000
};
const uint8_t GFXExtension::__O_[] PROGMEM =
{
	0b00111110,
	0b01000001,
	0b01000001,
	0b00111110,
	0b10000000
};
const uint8_t GFXExtension::__Oe[] PROGMEM =
{
	0b00111101,
	0b01000010,
	0b01000010,
	0b00111101,
	0b10000000
};
const uint8_t GFXExtension::__P[] PROGMEM =
{
	0b01111111,
	0b00001001,
	0b00001001,
	0b00000110,
	0b10000000
};
const uint8_t GFXExtension::__Q[] PROGMEM =
{
	0b00111110,
	0b01000001,
	0b01000001,
	0b00111110,
	0b01000000,
	0b10000000
};
const uint8_t GFXExtension::__R[] PROGMEM =
{
	0b01111111,
	0b00011001,
	0b00101001,
	0b01000110,
	0b10000000
};
const uint8_t GFXExtension::__S[] PROGMEM =
{
	0b01000110,
	0b01001001,
	0b01001001,
	0b00110001,
	0b10000000
};
const uint8_t GFXExtension::__T[] PROGMEM =
{
	0b00000001,
	0b00000001,
	0b01111111,
	0b00000001,
	0b00000001,
	0b10000000
};
const uint8_t GFXExtension::__U[] PROGMEM =
{
	0b00111111,
	0b01000000,
	0b01000000,
	0b00111111,
	0b10000000
};
const uint8_t GFXExtension::__Ue[] PROGMEM =
{
	0b00111100,
	0b01000001,
	0b01000001,
	0b00111100,
	0b10000000
};
const uint8_t GFXExtension::__V[] PROGMEM =
{
	0b00011111,
	0b00100000,
	0b01000000,
	0b00100000,
	0b00011111,
	0b10000000
};
const uint8_t GFXExtension::__W[] PROGMEM =
{
	0b00011111,
	0b00100000,
	0b01000000,
	0b00110000,
	0b01000000,
	0b00100000,
	0b00011111,
	0b10000000
};
const uint8_t GFXExtension::__X[] PROGMEM =
{
	0b01100011,
	0b00010100,
	0b00001000,
	0b00010100,
	0b01100011,
	0b10000000
};
const uint8_t GFXExtension::__Y[] PROGMEM =
{
	0b00000011,
	0b00000100,
	0b01111000,
	0b00000100,
	0b00000011,
	0b10000000
};
const uint8_t GFXExtension::__Z[] PROGMEM =
{
	0b01000001,
	0b01010001,
	0b01000101,
	0b01000001,
	0b10000000
};

const uint8_t GFXExtension::__a[] PROGMEM =
{
	0b00100000,
	0b01010100,
	0b01010100,
	0b01111000,
	0b10000000
};

const uint8_t GFXExtension::__ae[] PROGMEM =
{
	0b00100000,
	0b01010101,
	0b01010100,
	0b01111001,
	0b10000000
};

const uint8_t GFXExtension::__b[] PROGMEM =
{
	0b01111111,
	0b01000100,
	0b01000100,
	0b00111000,
	0b10000000
};

const uint8_t GFXExtension::__c[] PROGMEM =
{
	0b00111000,
	0b01000100,
	0b01000100,
	0b10000000
};

const uint8_t GFXExtension::__d[] PROGMEM =
{
	0b00111000,
	0b01000100,
	0b01000100,
	0b01111111,
	0b10000000
};

const uint8_t GFXExtension::__e[] PROGMEM =
{
	0b00111000,
	0b01010100,
	0b01010100,
	0b00011000,
	0b10000000
};

const uint8_t GFXExtension::__f[] PROGMEM =
{
	0b00000100,
	0b01111110,
	0b00000101,
	0b10000000
};

const uint8_t GFXExtension::__g[] PROGMEM =
{
	0b00001100,
	0b01010010,
	0b01010010,
	0b00111110,
	0b10000111
};

const uint8_t GFXExtension::__h[] PROGMEM =
{
	0b01111111,
	0b00000100,
	0b00000100,
	0b01111000,
	0b10000000
};

const uint8_t GFXExtension::__i[] PROGMEM =
{
	0b01111101,
	0b10000000
};

const uint8_t GFXExtension::__j[] PROGMEM =
{
	0b01000000,
	0b01000000,
	0b00111101,
	0b10000111
};

const uint8_t GFXExtension::__k[] PROGMEM =
{
	0b01111111,
	0b00010000,
	0b00101000,
	0b01000100,
	0b10000000
};

const uint8_t GFXExtension::__l[] PROGMEM =
{
	0b00111111,
	0b01000000,
	0b01000000,
	0b10000000
};

const uint8_t GFXExtension::__m[] PROGMEM =
{
	0b01111000,
	0b00000100,
	0b01111000,
	0b00000100,
	0b01111000,
	0b10000000
};

const uint8_t GFXExtension::__n[] PROGMEM =
{
	0b01111100,
	0b00000100,
	0b00000100,
	0b01111000,
	0b10000000
};

const uint8_t GFXExtension::__o[] PROGMEM =
{
	0b00111000,
	0b01000100,
	0b01000100,
	0b00111000,
	0b10000000
};
const uint8_t GFXExtension::__oe[] PROGMEM =
{
	0b00111001,
	0b01000100,
	0b01000100,
	0b00111001,
	0b10000000
};

const uint8_t GFXExtension::__p[] PROGMEM =
{
	0b01111100,
	0b00100100,
	0b00100100,
	0b00011000,
	0b10000000
};

const uint8_t GFXExtension::__q[] PROGMEM =
{
	0b00011000,
	0b00100100,
	0b00100100,
	0b01111100,
	0b10000000
};

const uint8_t GFXExtension::__r[] PROGMEM =
{
	0b01111100,
	0b00000100,
	0b00001000,
	0b10000000
};

const uint8_t GFXExtension::__s[] PROGMEM =
{
	0b00001000,
	0b01010100,
	0b01010100,
	0b00100000,
	0b10000000
};

const uint8_t GFXExtension::__t[] PROGMEM =
{
	0b00000100,
	0b00111111,
	0b01000100,
	0b10000000
};

const uint8_t GFXExtension::__u[] PROGMEM =
{
	0b00111100,
	0b01000000,
	0b01000000,
	0b00111100,
	0b10000000
};
const uint8_t GFXExtension::__ue[] PROGMEM =
{
	0b00111010,
	0b01000000,
	0b01000000,
	0b00111010,
	0b10000000
};

const uint8_t GFXExtension::__v[] PROGMEM =
{
	0b00011100,
	0b00100000,
	0b01000000,
	0b00100000,
	0b00011100,
	0b10000000
};

const uint8_t GFXExtension::__w[] PROGMEM =
{
	0b01111100,
	0b00100000,
	0b00010000,
	0b00100000,
	0b01111100,
	0b10000000
};

const uint8_t GFXExtension::__x[] PROGMEM =
{
	0b01000100,
	0b00101000,
	0b00010000,
	0b00101000,
	0b01000100,
	0b10000000
};

const uint8_t GFXExtension::__y[] PROGMEM =
{
	0b00001100,
	0b01010000,
	0b01010000,
	0b00111100,
	0b10000000
};

const uint8_t GFXExtension::__z[] PROGMEM =
{
	0b01100100,
	0b01010100,
	0b01010100,
	0b01001100,
	0b10000000
};

const uint8_t GFXExtension::__0[] PROGMEM =
{
	0b00111110,
	0b01010001,
	0b01000101,
	0b00111110,
	0b10000000
};
const uint8_t GFXExtension::__1[] PROGMEM =
{
	0b01000010,
	0b01000001,
	0b01111111,
	0b01000000,
	0b10000000
};

const uint8_t GFXExtension::__2[] PROGMEM =
{
	0b01100010,
	0b01010001,
	0b01001001,
	0b01000110,
	0b10000000
};
const uint8_t GFXExtension::__3[] PROGMEM =
{
	0b01000001,
	0b01001001,
	0b01001001,
	0b00110110,
	0b10000000
};
const uint8_t GFXExtension::__4[] PROGMEM =
{
	0b00011111,
	0b00010000,
	0b01111100,
	0b00010000,
	0b10000000
};
const uint8_t GFXExtension::__5[] PROGMEM =
{
	0b01001111,
	0b01001001,
	0b01001001,
	0b00110001,
	0b10000000
};
const uint8_t GFXExtension::__6[] PROGMEM =
{
	0b00111110,
	0b01001001,
	0b01001001,
	0b00110000,
	0b10000000
};
const uint8_t GFXExtension::__7[] PROGMEM =
{
	0b01110001,
	0b00001001,
	0b00000101,
	0b00000011,
	0b10000000
};
const uint8_t GFXExtension::__8[] PROGMEM =
{
	0b00110110,
	0b01001001,
	0b01001001,
	0b00110110,
	0b10000000
};
const uint8_t GFXExtension::__9[] PROGMEM =
{
	0b00000110,
	0b01001001,
	0b01001001,
	0b00111110,
	0b10000000
};

const uint8_t* const GFXExtension::__ABC[] PROGMEM =
{
	__A, __B, __C, __D, __E, __F, __G, __H, __I_, __J, __K, __L, __M, __N, __O_, __P, __Q, __R, __S, __T, __U, __V, __W, __X, __Y, __Z
};
const uint8_t* const GFXExtension::__abc[] PROGMEM =
{
	__a, __b, __c, __d, __e, __f, __g, __h, __i, __j, __k, __l, __m, __n, __o, __p, __q, __r, __s, __t, __u, __v, __w, __x, __y, __z
};
const uint8_t* const GFXExtension::__0123[] PROGMEM =
{
	__0, __1, __2, __3, __4, __5, __6, __7, __8, __9
};
