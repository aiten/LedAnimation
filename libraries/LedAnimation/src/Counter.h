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

#include "Animation.h"

class CCounter : public CAnimation
{
private:
	typedef CAnimation super;

	int _delayms;

	uint16_t _countFrom;
	uint16_t _countTo;

	byte       _colorIdx;
	const byte NOFORCECOLORIDX = 255;
	byte       _forceColorIdx;

	void ClockLoop(uint16_t countuntil, bool countupanddown, int delayms);

	uint16_t NextColor();

	int LimitCounter(int counter)
	{
		return counter > 32000 ? 32000 : counter;
	}

public:
	CCounter(uint32_t showtime = 10000, uint16_t countFrom = 0, uint16_t countTo = 99, byte colorIdx = 255, int delayms = 1000) : super(showtime)
	{
		_countFrom     = LimitCounter(countFrom);
		_countTo       = LimitCounter(countTo);
		_delayms       = delayms;
		_forceColorIdx = (colorIdx < GFXExtension::ColorPaletteSize) ? colorIdx : NOFORCECOLORIDX;
	}

	int  GetDelay() { return _delayms; }
	void SetDelay(int delayms) { _delayms = delayms; }

	virtual void Show();
};
