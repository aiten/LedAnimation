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

class CWheel : public CAnimation
{
private:
	typedef CAnimation super;

public:
	CWheel(uint32_t showtime = 10000) : super(showtime)
	{
	}

	virtual void Show()
	{
		uint16_t counter  = 0;
		uint32_t exittime = millis() + GetShowTime();

		while (millis() < exittime)
		{
			counter++;
			if (counter > 2000)
			{
				counter = 0;
			}

			uint16_t counter3 = counter * 1;

			for (uint16_t counter2 = 0; counter2 < (uint16_t)(GFX()->width() * GFX()->height()); counter2++)
			{
				//      Display[counter2+OFS_LEDS] = Wheel(counter3%96);  //There's only 96 colors in this pallette.
				//      counter3+=(96 / USE_LEDS);
				GFX()->drawPixel(counter2 % GFX()->width(), counter2 / GFX()->width(), GFXExtension::Wheel(counter3 % 96));
				//There's only 96 colors in this pallette.

				if (counter2 % 16 == 0)
				{
					counter3 += (counter2 % 6) == 0 ? 1 : 0;
				}
			}
			if (!ShowAndDelay(100))
				return;
		}
	}
};
