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

#include "Ticker.h"

///////////////////////////////////////////////////////////////

uint16_t CTicker::Gap(uint16_t delayTime)
{
	for (uint8_t i = 0; i < Graphic()->GetCharSizeMult(); i++)
	{
		Graphic()->ScrollLeft(_fillColor);
		ShowAndDelay(delayTime);
	}

	return NextPallet16Color(_textPalletColor);
}

///////////////////////////////////////////////////////////////

void CTicker::Show()
{
	GFX()->fillScreen(_fillColor);

	uint16_t col = NextPallet16Color(_textPalletColor);

	int delayTimeMove;

	if (_timeToMove == 0)
	{
		if (_style == ScrollSingleCharFadeInRightFadeOutRight || _style == ScrollSingleCharFadeInRightFadeOutLeft ||
			_style == ScrollSingleCharFadeInRightFadeOutUp || _style == ScrollSingleCharFadeInRightFadeOutDown)
		{
			delayTimeMove = TimeToDelayHorizontal(1000);
		}
		else
		{
			delayTimeMove = TimeToDelayHorizontal(2000);
		}
	}
	else
	{
		delayTimeMove = TimeToDelayHorizontal(_timeToMove);
	}

	const char* ch = _text;

	while (GetChar(ch) != '\0' && AskContinue())
	{
		uint8_t drawSize = Graphic()->ScrollInRight(GetChar(ch), delayTimeMove, GetAskContinue(), col, _fillColor);

		if (_style == ScrollFromRight)
		{
			col = Gap(delayTimeMove);
		}
		else
		{
			uint8_t dist = (GFX()->width() - drawSize) / 2;
			Graphic()->ScrollRangeLeft(dist, delayTimeMove, GetAskContinue(), _fillColor);
			if (!ShowAndDelay(_delayms))
				return;

			switch (_style)
			{
			default:
			case ScrollSingleCharFadeInRightFadeOutRight:
				Graphic()->ScrollRangeRight(dist + drawSize, delayTimeMove, GetAskContinue(), _fillColor);
				break;
			case ScrollSingleCharFadeInRightFadeOutLeft:
				Graphic()->ScrollRangeLeft(((GFX()->width() - 1 + drawSize)) - (dist + drawSize), delayTimeMove, GetAskContinue(), _fillColor);
				break;
			case ScrollSingleCharFadeInRightFadeOutUp:
				Graphic()->ScrollRangeUp(GFX()->height(), delayTimeMove, GetAskContinue(), _fillColor);
				break;
			case ScrollSingleCharFadeInRightFadeOutDown:
				Graphic()->ScrollRangeDown(GFX()->height(), delayTimeMove, GetAskContinue(), _fillColor);
				break;
			}
			col = Gap(delayTimeMove);
		}
		ch++;
	}

	if (_style == ScrollFromRight)
	{
		Graphic()->ScrollRangeLeft((GFX()->width() - 1), delayTimeMove, GetAskContinue(), _fillColor);
	}
}
