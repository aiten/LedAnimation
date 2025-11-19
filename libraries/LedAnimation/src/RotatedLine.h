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

class CRotatedLine : public CAnimation
{
private:
	typedef CAnimation super;

public:
	CRotatedLine(uint32_t showtime = 30000) : super(showtime)
	{
	}

	virtual void Show()
	{
		GFX()->fillScreen(GFXExtension::Color565(0, 0, 0));

		uint32_t exitTime  = millis() + GetShowTime();
		uint16_t delayTime = TimeToDelayHorizontal(1000);

		int x=0;
		int dx = 1;
		int y = 0;
		int dy = 0;
		byte weelPos=0;
		

		while (millis() < exitTime)
		{
			x += dx;
			if (x >= GFX()->width())
			{
				x--;
				dx = 0;
				dy = 1;
			}
			if (x < 0)
			{
				x++;
				dx = 0;
				dy = -1;
			}

			y += dy;
			if (y >= GFX()->height())
			{
				y--;
				dx = -1;
				dy = 0;
			}
			if (y < 0)
			{
				y++;
				x = 1;
				dx = 1;
				dy = 0;
			}

			auto color = GFXExtension::Wheel((weelPos++) % 96);

			GFX()->drawLine(x, y, GFX()->width()-x -1, GFX()->height() - y-1, GFXExtension::Color565(255, 255, 255));

			if (!ShowAndDelay(delayTime))
			{
				return;
			}

			GFX()->drawLine(x, y, GFX()->width() - x - 1, GFX()->height() - y - 1, color);
		}
	}
};
