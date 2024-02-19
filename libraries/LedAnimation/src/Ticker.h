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
#include "GFXExtension.h"

class CTicker : public CAnimation
{
private:
	typedef CAnimation super;

public:
	enum ETickerType
	{
		ScrollFromRight,
		ScrollSingleCharFadeInRightFadeOutRight,
		ScrollSingleCharFadeInRightFadeOutLeft,
		ScrollSingleCharFadeInRightFadeOutUp,
		ScrollSingleCharFadeInRightFadeOutDown,
		Scroll5,
		Scroll6
	};

private:
	const char* _text;
	bool        _isProgmem;
	ETickerType _style;
	uint16_t    _delayms;
	uint16_t    _timeToMove;

	uint16_t _fillColor       = GFXExtension::OffColor;
	uint8_t  _textPalletColor = GFXExtension::NoPallet16Color;

	char     GetChar(const char* text) { return _isProgmem ? pgm_read_byte(text) : *text; }
	uint16_t Gap(uint16_t delayTime);

public:
	CTicker(const char* text, ETickerType style = ScrollFromRight, uint16_t timeToMove = 0, int stopDelay = 1000) : super(0)
	{
		_text       = text;
		_isProgmem  = false;
		_style      = style;
		_timeToMove = timeToMove;
		_delayms    = stopDelay;
	};
	/*
		CTicker(const char* text = NULL, ETickerType style = ScrollFromRight, int delayms = 1000, int scrolldelay = -1) : super(0)
		{
			_text = text;
			_isProgmem = true;
			_style = style;
			_delayms = delayms;
			_scrolldelay = scrolldelay;
		};
	*/
#ifndef _MSC_VER

	CTicker(const __FlashStringHelper* text = NULL, ETickerType style = ScrollFromRight, uint16_t timeToMove = 0, int stopDelay = 1000) : super(0)
	{
		_text		= (const char*) text; 
		_isProgmem  = true;
		_style		= style; 
		_style = style;
		_timeToMove = timeToMove;
		_delayms = stopDelay;
	};

#endif

	const char* GetText() { return _text; }
	void        SetText(char* text) { _text = text; }

	ETickerType GetTickerType() { return _style; }
	void        SetTickerType(ETickerType style) { _style = style; }

	uint16_t GetDelay() { return _delayms; }
	void     SetDelay(uint16_t delayms) { _delayms = delayms; }

	uint16_t GetTimeToMove() { return _timeToMove; }
	void     SetTimeToMove(uint16_t scrolldelay) { _timeToMove = scrolldelay; }

	uint16_t GetFillColor() { return _fillColor; }
	void     SetFillColor(uint16_t color) { _fillColor = color; }

	uint8_t GetTextColorIdx() { return _textPalletColor; }
	void    SetTextColorIdx(uint8_t color) { _textPalletColor = color; }

	virtual void Show();
};
