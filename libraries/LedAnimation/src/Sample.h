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

class CSample : public CAnimation
{
private:
	typedef CAnimation super;

public:
	CSample(uint32_t showtime = 30000) : super(showtime)
	{
	}

	virtual void Show()
	{
		GFX()->fillScreen(GFXExtension::Color565(40, 0, 0));

		uint32_t exitTime  = millis() + GetShowTime();
		uint16_t delayTime = 300;

		int count=0;

		while (millis() < exitTime)
		{
			GFX()->drawChar(0, 0, 'a' + (count++) % 26, GFXExtension::Color565(255, 255, 0), GFXExtension::Color565(0, 0, 0), 1,1);

			if (!Delay(delayTime))
			{
				return;
			}
		}
	}
};
