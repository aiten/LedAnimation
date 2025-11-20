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

#pragma once

#include <Adafruit_GFX.h>
#include "AskContinue.h"

#define CHARHEIGHT	8

class GFXExtension
{
private:
	Adafruit_GFX* _adafruitGFX;

public:
	Adafruit_GFX* GFX() { return _adafruitGFX; }

	static constexpr uint16_t Color565Const(uint8_t r, uint8_t g, uint8_t b)
	{
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}

	static constexpr uint32_t Color32Const(uint8_t r, uint8_t g, uint8_t b)
	{
		return (((uint32_t)g) << 16) + (((uint32_t)b) << 8) + r;
	}

	static constexpr uint8_t Color565Red(uint16_t col) { return (col >> 11) & 0x1f; }
	static constexpr uint8_t Color565Green(uint16_t col) { return (col >> 5) & 0x3f; }
	static constexpr uint8_t Color565Blue(uint16_t col) { return col & 0x1f; }

	static void Color32ToRGB(uint32_t col, uint8_t& r, uint8_t& g, uint8_t& b);

	static uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) { return Color565Const(r, g, b); };
	static uint32_t Color32(uint8_t r, uint8_t g, uint8_t b) { return Color32Const(r, g, b); };

	static uint16_t Color32To565(uint32_t color);
	static uint32_t Expand565To32(uint16_t color);

	static uint16_t Wheel(byte WheelPos);

protected:
	GFXExtension(Adafruit_GFX* gfx)
	{
		_adafruitGFX = gfx;

		if (_charSizeMult == 0)
		{
			_charSizeMult = max(1, GFX()->height() / (CHARHEIGHT+1));
		}
	}

public:
	static const uint16_t NoColor;
	static const uint16_t OffColor;

	static const uint8_t NoPallet16Color;

	virtual void     Show(bool force) =0;
	virtual uint16_t getPixel(uint16_t x, uint16_t y) =0;

	enum EColorPalett16Colors
	{
		Black = 0,
		White,
		Red,
		Lime,
		Blue,
		Yellow,
		Cyan,
		Magenta,
		DarkOrange,
		Gray,
		Maroon,
		Olive,
		Green,
		Purple,
		Teal,
		Navy,
		ColorPaletteSize = Navy + 1
	};

	uint16_t Count(uint16_t color);

	static uint32_t GetColorPalette32(uint8_t colorIdx);
	static uint16_t GetColorPalette565(uint8_t colorIdx) { return Color32To565(GetColorPalette32(colorIdx)); }

	void Scroll(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, signed char dx, signed char dy, uint16_t fillcol = NoColor);

	void ScrollLeft(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol = NoColor);
	void ScrollRight(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol = NoColor);
	void ScrollUp(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol = NoColor);
	void ScrollDown(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t fillcol = NoColor);

	void Scroll(char dx, char dy, uint16_t fillcol = NoColor) { Scroll(0, 0, GFX()->width() - 1, GFX()->height() - 1, dx, dy, fillcol); };
	void ScrollLeft(uint16_t fillcol = NoColor) { ScrollLeft(0, 0, GFX()->width() - 1, GFX()->height() - 1, fillcol); };
	void ScrollRight(uint16_t fillcol = NoColor) { ScrollRight(0, 0, GFX()->width() - 1, GFX()->height() - 1, fillcol); };
	void ScrollUp(uint16_t fillcol = NoColor) { ScrollUp(0, 0, GFX()->width() - 1, GFX()->height() - 1, fillcol); };
	void ScrollDown(uint16_t fillcol = NoColor) { ScrollDown(0, 0, GFX()->width() - 1, GFX()->height() - 1, fillcol); };

	void FadeOut(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned int delayTime);

	void ScrollRange(uint8_t d, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int8_t dx, int8_t dy, uint16_t delayTime, CAskContinue* askContinue, uint16_t col = NoColor);

	void ScrollRangeLeft(uint8_t d, uint16_t delayTime, CAskContinue* askContinue, uint16_t col = NoColor) { ScrollRange(d, 0, 0, GFX()->width() - 1, GFX()->height() - 1, -1, 0, delayTime, askContinue, col); };
	void ScrollRangeRight(uint8_t d, uint16_t delayTime, CAskContinue* askContinue, uint16_t col = NoColor) { ScrollRange(d, 0, 0, GFX()->width() - 1, GFX()->height() - 1, 1, 0, delayTime, askContinue, col); };
	void ScrollRangeUp(uint8_t d, uint16_t delayTime, CAskContinue* askContinue, uint16_t col = NoColor) { ScrollRange(d, 0, 0, GFX()->width() - 1, GFX()->height() - 1, 0, 1, delayTime, askContinue, col); };
	void ScrollRangeDown(uint8_t d, uint16_t delayTime, CAskContinue* askContinue, uint16_t col = NoColor) { ScrollRange(d, 0, 0, GFX()->width() - 1, GFX()->height() - 1, 0, -1, delayTime, askContinue, col); };

	/// font

	uint8_t ScrollInRight(char ch, uint16_t delay, CAskContinue* askContinue, uint16_t col, uint16_t fillcol = OffColor);

	uint8_t GetCharSizeMult() { return _charSizeMult; }

private:
	static const uint32_t __myColorPalette16[] PROGMEM;

	static uint8_t _charSizeMult;

	/// font

	static const uint8_t* ToAsciiArray(char ch);
	static const uint8_t* ToAsciiArray(char ch, uint8_t& charXsize, int8_t& shift);

	uint8_t PrintChar(char ch, uint16_t x, uint16_t y, uint16_t col, uint16_t fillcol = OffColor);
	uint8_t PrintChar(char ch, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col, uint16_t fillcol = OffColor); //center in area

	static const uint8_t __0[] PROGMEM;
	static const uint8_t __1[] PROGMEM;
	static const uint8_t __2[] PROGMEM;
	static const uint8_t __3[] PROGMEM;
	static const uint8_t __4[] PROGMEM;
	static const uint8_t __5[] PROGMEM;
	static const uint8_t __6[] PROGMEM;
	static const uint8_t __7[] PROGMEM;
	static const uint8_t __8[] PROGMEM;
	static const uint8_t __9[] PROGMEM;

	static const uint8_t __BracketO[] PROGMEM;
	static const uint8_t __BracketC[] PROGMEM;
	static const uint8_t __Blank[] PROGMEM;
	static const uint8_t __Dot[] PROGMEM;
	static const uint8_t __Comma[] PROGMEM;
	static const uint8_t __Minus[] PROGMEM;
	static const uint8_t __Plus[] PROGMEM;
	static const uint8_t __Mult[] PROGMEM;
	static const uint8_t __Div[] PROGMEM;
	static const uint8_t __Exp[] PROGMEM;
	static const uint8_t __Qest[] PROGMEM;
	static const uint8_t __A[] PROGMEM;
	static const uint8_t __B[] PROGMEM;
	static const uint8_t __C[] PROGMEM;
	static const uint8_t __D[] PROGMEM;
	static const uint8_t __E[] PROGMEM;
	static const uint8_t __F[] PROGMEM;
	static const uint8_t __G[] PROGMEM;
	static const uint8_t __H[] PROGMEM;
	static const uint8_t __I_[] PROGMEM;
	static const uint8_t __J[] PROGMEM;
	static const uint8_t __K[] PROGMEM;
	static const uint8_t __L[] PROGMEM;
	static const uint8_t __M[] PROGMEM;
	static const uint8_t __N[] PROGMEM;
	static const uint8_t __O_[] PROGMEM;
	static const uint8_t __P[] PROGMEM;
	static const uint8_t __Q[] PROGMEM;
	static const uint8_t __R[] PROGMEM;
	static const uint8_t __S[] PROGMEM;
	static const uint8_t __T[] PROGMEM;
	static const uint8_t __U[] PROGMEM;
	static const uint8_t __V[] PROGMEM;
	static const uint8_t __W[] PROGMEM;
	static const uint8_t __X[] PROGMEM;
	static const uint8_t __Y[] PROGMEM;
	static const uint8_t __Z[] PROGMEM;
	static const uint8_t __a[] PROGMEM;
	static const uint8_t __b[] PROGMEM;
	static const uint8_t __c[] PROGMEM;
	static const uint8_t __d[] PROGMEM;
	static const uint8_t __e[] PROGMEM;
	static const uint8_t __f[] PROGMEM;
	static const uint8_t __g[] PROGMEM;
	static const uint8_t __h[] PROGMEM;
	static const uint8_t __i[] PROGMEM;
	static const uint8_t __j[] PROGMEM;
	static const uint8_t __k[] PROGMEM;
	static const uint8_t __l[] PROGMEM;
	static const uint8_t __m[] PROGMEM;
	static const uint8_t __n[] PROGMEM;
	static const uint8_t __o[] PROGMEM;
	static const uint8_t __p[] PROGMEM;
	static const uint8_t __q[] PROGMEM;
	static const uint8_t __r[] PROGMEM;
	static const uint8_t __s[] PROGMEM;
	static const uint8_t __t[] PROGMEM;
	static const uint8_t __u[] PROGMEM;
	static const uint8_t __v[] PROGMEM;
	static const uint8_t __w[] PROGMEM;
	static const uint8_t __x[] PROGMEM;
	static const uint8_t __y[] PROGMEM;
	static const uint8_t __z[] PROGMEM;

	static const uint8_t* const __ABC[] PROGMEM;
	static const uint8_t* const __abc[] PROGMEM;
	static const uint8_t* const __0123[] PROGMEM;

	static const uint8_t __ae[] PROGMEM;
	static const uint8_t __Ae[] PROGMEM;
	static const uint8_t __oe[] PROGMEM;
	static const uint8_t __Oe[] PROGMEM;
	static const uint8_t __ue[] PROGMEM;
	static const uint8_t __Ue[] PROGMEM;

	static const uint8_t __Sz[] PROGMEM;
};
