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

#include <Arduino.h>
#include "GFXExtension.h"

#if defined(__SAM3X8E__)

#define pgm_read_ptr pgm_read_dword

#else

//#define pgm_read_ptr pgm_read_word

#endif

const uint8_t PROGMEM
	gamma5[] = {
		0x00, 0x01, 0x02, 0x03, 0x05, 0x07, 0x09, 0x0b,
		0x0e, 0x11, 0x14, 0x18, 0x1d, 0x22, 0x28, 0x2e,
		0x36, 0x3d, 0x46, 0x4f, 0x59, 0x64, 0x6f, 0x7c,
		0x89, 0x97, 0xa6, 0xb6, 0xc7, 0xd9, 0xeb, 0xff
	},
	gamma6[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08,
		0x09, 0x0a, 0x0b, 0x0d, 0x0e, 0x10, 0x12, 0x13,
		0x15, 0x17, 0x19, 0x1b, 0x1d, 0x20, 0x22, 0x25,
		0x27, 0x2a, 0x2d, 0x30, 0x33, 0x37, 0x3a, 0x3e,
		0x41, 0x45, 0x49, 0x4d, 0x52, 0x56, 0x5b, 0x5f,
		0x64, 0x69, 0x6e, 0x74, 0x79, 0x7f, 0x85, 0x8b,
		0x91, 0x97, 0x9d, 0xa4, 0xab, 0xb2, 0xb9, 0xc0,
		0xc7, 0xcf, 0xd6, 0xde, 0xe6, 0xee, 0xf7, 0xff
	};

/******************************************************************************/

uint32_t GFXExtension::Expand565To32(uint16_t color)
{
	return
		((uint32_t)pgm_read_byte(&gamma5[Color565Red(color)]) << 16) |
		((uint32_t)pgm_read_byte(&gamma6[Color565Green(color)]) << 8) |
		pgm_read_byte(&gamma5[Color565Blue(color)]);
}

/******************************************************************************/

void GFXExtension::Color32ToRGB(uint32_t col, uint8_t& r, uint8_t& g, uint8_t& b)
{
	r = (col & 0xFF);
	g = ((col >> 16) & 0xFF);
	b = ((col >> 8) & 0xFF);
}

/******************************************************************************/

uint16_t GFXExtension::Color32To565(uint32_t col)
{
	uint8_t r, g, b;
	Color32ToRGB(col, r, g, b);
	return Color565(r, g, b);
}

uint32_t GFXExtension::GetColorPalette32(uint8_t colorIdx)
{
	return (uint32_t)pgm_read_dword(&__myColorPalette16[colorIdx]);
}

/******************************************************************************/

uint16_t GFXExtension::Count(uint16_t color)
{
	uint16_t width  = GFX()->width();
	uint16_t height = GFX()->height();
	uint16_t cnt    = 0;

	for (uint16_t x = 0; x < width; x++)
	{
		for (uint16_t y = 0; y < height; y++)
		{
			if (getPixel(x, y) == color)
			{
				cnt++;
			}
		}
	}
	return cnt;
}

/******************************************************************************/

const uint32_t GFXExtension::__myColorPalette16[] PROGMEM =
{
	0x000000, // 0x000000,		             //Black         #000000
	0xFFFFFF, // Color32Const(255, 255, 255), //White         #FFFFFF
	0xFF0000, // Color32Const(255, 0, 0),	 //Red           #FF0000
	0x00FF00, // Color32Const(0, 255, 0),	 //Lime          #00FF00
	0x0000FF, // Color32Const(0, 0, 255),	 //Blue          #0000FF
	0xFFFF00, // Color32Const(255, 255, 0),	 //Yellow        #FFFF00
	0x00FFFF, // Color32Const(0, 255, 255),	 //Cyan / Aqua   #00FFFF
	0xFF00FF, // Color32Const(255, 0, 255),	 //Magenta / Fuchsia   #FF00FF
	0xFF8C00, // Const(255, 140, 0),	    //DarkOrange
	0x808080, // Color32Const(1, 1, 1),	   //Gray            #808080
	0x800000, // Color32Const(128, 0, 0),   //Maroon          #800000
	0x808000, // Color32Const(128, 128, 0), //Olive           #808000
	0x008000, // Color32Const(0, 128, 0),   //Green           #008000
	0x800080, // Color32Const(128, 0, 128), //Purple          #800080
	0x008080, // Color32Const(0, 128, 128), //Teal            #008080
	0x000080  // Color32Const(0, 0, 128)	   //Navy            #000080
};

/******************************************************************************/

////Swap the values of two variables, for use when drawing lines.
void Swap(int* a, int* b)
{
	byte temp;
	temp = *b;
	*b   = *a;
	*a   = temp;
}

/******************************************************************************/

void GFXExtension::ScrollLeft(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol)
{
	for (uint16_t y = y0; y <= y1; y++)
	{
		for (uint16_t x = x0 + 1; x <= x1; x++)
		{
			GFX()->drawPixel(x - 1, y, getPixel(x, y));
		}
		if (fillcol != NoColor)
			GFX()->drawPixel(GFX()->width() - 1, y, fillcol);
	}
}

/******************************************************************************/

void GFXExtension::ScrollRight(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol)
{
	for (uint16_t y = y0; y <= y1; y++)
	{
		for (int16_t x = x1 - 1; x >= (int16_t)x0; x--)
		{
			GFX()->drawPixel(x + 1, y, getPixel(x, y));
		}

		if (fillcol != NoColor)
			GFX()->drawPixel(0, y, fillcol);
	}
}

/******************************************************************************/

void GFXExtension::ScrollUp(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol)
{
	for (uint16_t x = x0; x <= x1; x++)
	{
		for (uint16_t y = y0 + 1; y <= y1; y++)
		{
			GFX()->drawPixel(x, y - 1, getPixel(x, y));
		}

		if (fillcol != NoColor)
			GFX()->drawPixel(x, GFX()->height() - 1, fillcol);
	}
}

/******************************************************************************/

void GFXExtension::ScrollDown(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol)
{
	for (uint16_t x = x0; x <= x1; x++)
	{
		for (int16_t y = y1 - 1; y >= (int16_t)y0; y--)
		{
			GFX()->drawPixel(x, y + 1, getPixel(x, y));
		}

		if (fillcol != NoColor)
			GFX()->drawPixel(x, 0, fillcol);
	}
}

/******************************************************************************/

void GFXExtension::Scroll(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int8_t dx, int8_t dy, uint16_t fillcol)
{
	if (dx > 0)
		ScrollRight(x0, y0, x1, y1, fillcol);
	else if (dx < 0)
		ScrollLeft(x0, y0, x1, y1, fillcol);

	if (dy > 0)
		ScrollUp(x0, y0, x1, y1, fillcol);
	else if (dy < 0)
		ScrollDown(x0, y0, x1, y1, fillcol);
}

/******************************************************************************/

void GFXExtension::ScrollRange(uint8_t d, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int8_t dx, int8_t dy, uint16_t delayTime, CAskContinue* askContinue, uint16_t col)
{
	for (uint8_t b = 0; b < d; b++)
	{
		Scroll(x0, y0, x1, y1, dx, dy, col);
		Show(true);
		askContinue->Delay(delayTime);
	}
}

/******************************************************************************/

uint16_t GFXExtension::Wheel(byte WheelPos)
{
	uint8_t r, g, b;
	switch (WheelPos >> 5)
	{
	default:
	case 0:
		r = 31 - WheelPos % 32; //Red down
		g = WheelPos % 32;      // Green up
		b = 0;                  //blue off
		break;
	case 1:
		g = 31 - WheelPos % 32; //green down
		b = WheelPos % 32;      //blue up
		r = 0;                  //red off
		break;
	case 2:
		b = 31 - WheelPos % 32; //blue down
		r = WheelPos % 32;      //red up
		g = 0;                  //green off
		break;
	}
	return (GFXExtension::Color565(r * 8, g * 8, b * 8));
}
