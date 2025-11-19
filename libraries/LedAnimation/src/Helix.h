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

class CHelix : public CAnimation
{
private:
	typedef CAnimation super;

public:
	CHelix(uint32_t showtime = 30000) : super(showtime)
	{
	}

	virtual void Show()
	{
		GFX()->fillScreen(GFXExtension::Color565(40, 0, 0));

		uint32_t exittime = millis() + GetShowTime();

		uint8_t loopcount = 0;

		uint16_t old = _old;

		uint16_t xmin = 0;
		uint16_t xmax = GFX()->width() - 1;

		uint16_t ymin = 0;
		uint16_t ymax = GFX()->height() - 1;

		uint16_t delay = TimeToDelayHorizontal(350);

		enum EDirection
		{
			XPlus,
			XMinus,
			YPlus,
			YMinus
		} dir = XPlus;

		uint16_t x = 0;
		uint16_t y = 0;

		uint16_t xold = 0;
		uint16_t yold = 0;

		while (millis() < exittime)
		{
			GFX()->drawPixel(xold, yold, old);
			GFX()->drawPixel(x, y, _new);

			if (xmin > xmax || ymin > ymax)
			{
				GFX()->drawPixel(x, y, old);

				xmin = 0;
				xmax = GFX()->width() - 1;

				ymin = 0;
				ymax = GFX()->height() - 1;

				dir = XPlus;

				x = 0;
				y = 0;

				xold = 0;
				yold = 0;

				loopcount++;

				if (loopcount >= _count)
					break;

				switch (loopcount % 4)
				{
				case 0:
					old = _old;
					break;
				case 1:
					old = GFXExtension::Color565(255, 0, 0);
					break;
				case 2:
					old = GFXExtension::Color565(0, 255, 0);
					break;
				case 3:
					old = GFXExtension::Color565(0, 0, 255);
					break;
				}

				continue;
			}

			xold = x;
			yold = y;

			switch (dir)
			{
			case XPlus:
				x++;
				if (x >= xmax)
				{
					ymin++;
					dir = YPlus;
				}
				break;

			case XMinus:
				x--;
				if (x <= xmin)
				{
					ymax--;
					dir = YMinus;
				}
				break;

			case YPlus:
				y++;
				if (y >= ymax)
				{
					xmax--;
					dir = XMinus;
				}
				break;

			case YMinus:
				y--;
				if (y <= ymin)
				{
					xmin++;
					dir = XPlus;
				}
				break;
			}

			if (!ShowAndDelay(delay))
				return;
		}
	}

private:
	uint16_t _new = GFXExtension::Color565(255, 255, 255);
	uint16_t _old = GFXExtension::Color565(31, 31, 31);

	unsigned char _count = 4;
};
