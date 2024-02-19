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

#include "Counter.h"

///////////////////////////////////////////////////////////////

#define MAXDIGITS 5

///////////////////////////////////////////////////////////////

uint16_t CCounter::NextColor()
{
	if (_forceColorIdx == NOFORCECOLORIDX)
	{
		_colorIdx = (_colorIdx + 1) % GFXExtension::ColorPaletteSize;
		if (_colorIdx == GFXExtension::Black)
		{
			_colorIdx++;
		}
	}
	return GFXExtension::GetColorPalette565(_colorIdx);
}

///////////////////////////////////////////////////////////////

void CCounter::Show()
{
	_colorIdx = _forceColorIdx == NOFORCECOLORIDX ? 0 : _forceColorIdx;

	uint16_t counter = _countFrom;
	int8_t   delta   = _countFrom < _countTo ? 1 : -1;

	counter -= delta;

	uint16_t col[MAXDIGITS];
	for (int i = 0; i < MAXDIGITS; i++)
	{
		col[i] = NextColor();
	}

	uint16_t bcol = GFXExtension::Color565(0, 0, 0);

	GFX()->fillScreen(bcol);

	const uint32_t addTime1 = _delayms;
	const uint32_t addTime2 = (_delayms / GFX()->width()) * 95 / 100;

	uint32_t nextMilli1 = addTime1;
	uint32_t nextMilli2 = addTime2;

	uint32_t startMillis = millis();

	int lastLength = -1;
	int dOld[MAXDIGITS]{0};

	while (counter != _countTo)
	{
		counter += delta;

		int number = counter;
		int length = 0;
		int d[MAXDIGITS];

		if (number == 0)
		{
			d[0]   = '0';
			length = 1;
		}
		else
		{
			while (number > 0)
			{
				d[length++] = (number % 10) + '0';
				number /= 10;
			}
		}

		if (lastLength != length)
		{
			GFX()->fillScreen(bcol);
			lastLength = length;
		}

		int size       = GFX()->height() / 9;
		size           = max(size, 1);
		int fontHeight = size * 7;
		int fontWidth  = size * 6; // incl. space
		int shiftV     = (GFX()->height() - fontHeight) / 2;
		int shiftH     = (GFX()->width() - (length * fontWidth)) / 2;

		for (int i = 0; i < length; i++)
		{
			if (d[i] != dOld[i])
			{
				col[i]  = NextColor();
				dOld[i] = d[i];
			}
			GFX()->drawChar(shiftH + (length - i - 1) * fontWidth, shiftV, d[i], col[i], bcol, size);
		}

		bool drawMyLine = _delayms >= 500;

		int      lineNo  = GFX()->height() - 1;
		uint16_t width   = GFX()->width();
		byte     barSize = 0;

		while ((GetShowTime() == 0 || (millis() < startMillis + GetShowTime())) && (millis() - startMillis) < nextMilli1)
		{
			if (drawMyLine && (millis() - startMillis) > nextMilli2 && barSize < width)
			{
				int rgb = 255l * barSize / width;
				GFX()->drawPixel(barSize, lineNo, GFXExtension::Color565(rgb, rgb, rgb));
				barSize++;
				nextMilli2 += addTime2;
			}
			if (!Delay(0))
				return;
		}

		nextMilli2 = nextMilli1 + addTime2;
		nextMilli1 += addTime1;

		if (drawMyLine)
		{
			GFX()->drawLine(0, lineNo, GFX()->width() - 1, lineNo, bcol);
		}
	}
}
