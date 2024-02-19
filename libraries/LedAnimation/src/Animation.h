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

#include "AskContinue.h"
#include "GFXExtension.h"

class CAnimation
{
private:
	uint32_t _showtime;
	bool     _enabled = true;

	static GFXExtension* _graphics;

	CAskContinue _askContinue;

public:
	static void InitGlobal(GFXExtension* graphics)
	{
		_graphics = graphics;
	}

	CAnimation(uint32_t showtime)
	{
		_showtime = showtime;
	}

	virtual void Show() = 0;

	uint32_t GetShowTime() { return _showtime; }
	void     SetShowTime(uint32_t showtime) { _showtime = showtime; }

	bool IsEnabled() { return _enabled; }
	void SetEnabled(bool enabled) { _enabled = enabled; }

	void          SetAskContinue(int param, bool (*askContinue)(int)) { _askContinue.SetAskContinue(param, askContinue); }
	CAskContinue* GetAskContinue() { return &_askContinue; }

	void SetAskContinue(CAnimation* animation) { _askContinue = animation->_askContinue; }
	bool AskContinue() const { return _askContinue.AskContinue(); }

	bool Delay(uint32_t delayMs) const
	{
		return _askContinue.Delay(delayMs);
	}

	void Flush();

	Adafruit_GFX* GFX() { return _graphics->GFX(); }
	GFXExtension* Graphic() { return _graphics; }

public:
	// helpers

	uint16_t NextPallet16Color(uint8_t overrideColorIndex);

	uint16_t TimeToDelayHorizontal(uint16_t timeInMs);
	uint16_t TimeToDelayVertical(uint16_t timeInMs);
};
