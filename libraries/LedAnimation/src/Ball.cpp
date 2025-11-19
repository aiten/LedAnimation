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

#include "Ball.h"

///////////////////////////////////////////////////////////////

#define RANDOMCHANGE 100
#define SNAKELENGTH 14
#define RANDOMXY 4

///////////////////////////////////////////////////////////////

static int8_t NextPos(int16_t& pos, int16_t& d, uint16_t MAXSIZE)
{
	pos += d;

	if (pos >= (int16_t)MAXSIZE)
	{
		d   = -1;
		pos = MAXSIZE - 2;
		return 1;
	}
	else if (pos < 0)
	{
		d   = 1;
		pos = 1;
		return -1;
	}
	return 0;
}

class CSnake
{
	uint16_t _cols[SNAKELENGTH];
	int16_t  _last[SNAKELENGTH];
	int16_t  _x  = -1;
	int16_t  _y  = -1;
	int16_t  _dx = 1;
	int16_t  _dy = 1;
	uint16_t _width;
	uint16_t _height;

	uint16_t _pixelColor;
	uint32_t _nextTime;

	uint8_t _lastidx = 0;
	uint8_t _delay;
	uint8_t _delay2;

	CBall* _ball;

	void CalcNextPos()
	{
		int16_t lastx = _x;
		//int16_t lasty = _y;

		int8_t res = NextPos(_x, _dx, _width);
		if (res != 0)
		{
			if (_dy == 0)
			{
				_dy = random(0, 2) == 0 ? 1 : -1; //-res;
			}
			else if (random(0, RANDOMXY) == 0)
			{
				_dy = 0;
			}
		}

		res = NextPos(_y, _dy, _height);
		if (res != 0)
		{
			if (_dx == 0)
			{
				_dx = random(0, 2) == 0 ? 1 : -1; // -res;
				NextPos(_x, _dx, _width);
			}
			else if (random(0, RANDOMXY) == 0)
			{
				_x  = lastx;
				_dx = 0;
			}
		}
	}

public:
	void Init(CBall* ball, uint16_t w, uint16_t h, uint16_t setColor, uint8_t delay)
	{
		_delay  = delay;
		_delay2 = (uint8_t)(((uint16_t)delay * 100) / 141);
		_ball   = ball;
		_height = h;
		_width  = w;

		uint8_t colrgb = 255;

		_x = random(0, w);
		_y = random(0, h);

		int8_t j = 0;

		_cols[j++] = GFXExtension::Color565(255, 0, 0);
		_cols[j++] = GFXExtension::Color565(0, 0, 255);

		while (j < SNAKELENGTH - 2)
		{
			_cols[j++] = GFXExtension::Color565(colrgb - 1, colrgb - 1, colrgb - 1);
			colrgb     = max(uint8_t(colrgb - 20), uint8_t(31));
		}

		_cols[j++] = GFXExtension::Color565(0, 255, 0);
		_cols[j++] = setColor;

		for (j = 0; j < SNAKELENGTH - 1; j++)
		{
			_last[j] = -1;
		}

		_nextTime = millis();
	}

	void Restart()
	{
		_nextTime = millis();
	}

	uint16_t GetLastPixelColor() { return _pixelColor; }

	bool Loop()
	{
		if (_nextTime > millis())
		{
			return false;
		}

		CalcNextPos();
		_pixelColor = _ball->Graphic()->getPixel(_x, _y);

		_nextTime += (_dx == 0 || _dy == 0) ? _delay2 : _delay;

		_last[_lastidx] = _x + _y * _width;
		_lastidx        = (_lastidx + 1) % SNAKELENGTH;

		for (uint8_t j = 0; j < SNAKELENGTH; j++)
		{
			int16_t idx = _last[(_lastidx + j) % SNAKELENGTH];
			if (idx != -1)
			{
				_ball->GFX()->drawPixel(idx % _width, idx / _width, _cols[SNAKELENGTH - 1 - j]);
			}
		}

		return true;
	}
};

///////////////////////////////////////////////////////////////

void CBall::Show()
{
	byte     backcolorsidx = 1;
	uint16_t backcolors[]  =
	{
		GFXExtension::Color565(32, 0, 0),
		GFXExtension::Color565(0, 0, 32),
		GFXExtension::Color565(0, 32, 0),
		GFXExtension::Color565(32, 32, 0),
		GFXExtension::Color565(32, 0, 32)
	};
	uint16_t backcolold = backcolors[backcolorsidx - 1];

	GFX()->fillScreen(backcolold);

	CSnake snake[_snakeCount];

	uint8_t minDelay    = TimeToDelayHorizontal(_fullMoveTime);
	uint8_t randomDelay = TimeToDelayHorizontal(_randomMoveTime);

	for (uint8_t i = 0; i < _snakeCount; i++)
	{
		snake[i].Init(this, GFX()->width(), GFX()->height(), GFXExtension::Color565(0, 0, 0), minDelay + random(0, randomDelay));
	}

	uint32_t exitTime = millis() + GetShowTime();

	while (millis() < exitTime)
	{
		bool colFound = false;
		for (uint8_t i = 0; i < _snakeCount; i++)
		{
			if (snake[i].Loop())
			{
				colFound = colFound || snake[i].GetLastPixelColor() == backcolold;
			}
		}

		if (colFound && Graphic()->Count(backcolold) == 0)
		{
			backcolold    = backcolors[backcolorsidx];
			backcolorsidx = (backcolorsidx + 1) % (sizeof(backcolors) / sizeof(uint16_t));

			if (!ShowAndDelay(1000) || _exitIfAll)
			{
				return;
			}
			GFX()->fillScreen(backcolold);
			for (uint8_t i = 0; i < _snakeCount; i++)
			{
				snake[i].Restart();
			}
		}

		if (!ShowAndDelay(0))
		{
			return;
		}
	}
}
