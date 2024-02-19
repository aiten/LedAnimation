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

class CAskContinue
{
private:
	int    _param;
	bool (*_askContinue)(int param) = nullptr;

public:
	CAskContinue()
	{
	}

	CAskContinue(const CAskContinue& src)
	{
		_param       = src._param;
		_askContinue = src._askContinue;
	}

	void SetAskContinue(int param, bool (*askContinue)(int))
	{
		_param       = param;
		_askContinue = askContinue;
	}

	bool AskContinue() const
	{
		if (_askContinue == nullptr)
		{
			return true;
		}

		return _askContinue(_param);
	}

	bool Delay(uint32_t delayMs) const
	{
		//		Serial.print(millis());
		//		Serial.print(':');
		//		Serial.println(delayMs);
		uint32_t until = millis() + delayMs;
		while (until > millis())
		{
			if (!AskContinue())
			{
				return false;
			}
		}
		return AskContinue();
	}
};
