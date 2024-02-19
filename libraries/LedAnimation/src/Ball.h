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

class CBall : public CAnimation
{
private:
	typedef CAnimation super;

private:
	bool     _exitIfAll;
	uint16_t _randomMoveTime;
	uint16_t _fullMoveTime;
	uint8_t  _snakeCount;

public:
	CBall(uint32_t showtime = 10000, uint8_t snakeCount = 1, bool exitIfAll = true, uint16_t fullMoveTime = 2000, uint16_t randomMoveTime = 0) : super(showtime)
	{
		_exitIfAll      = exitIfAll;
		_randomMoveTime = randomMoveTime;
		_fullMoveTime   = fullMoveTime;
		_snakeCount     = snakeCount;
	}

	bool GetExitIfAll() { return _exitIfAll; }
	void SetExitIfAll(bool exitIfAll) { _exitIfAll = exitIfAll; }

	virtual void Show();
};
