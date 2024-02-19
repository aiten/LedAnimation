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
#include <FastLED.h>
#include "GFXExtension.h"

#include "Fire.h"
//#include "lib8tion.h"

///////////////////////////////////////////////////////////////

#define BRIGHTNESS 200

///////////////////////////////////////////////////////////////

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING 60

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

///////////////////////////////////////////////////////////////

void CFire::CalcFire(byte* heat, uint16_t posX, uint16_t sizeX, uint16_t numLed)
{
	// Step 1.  Cool down every cell a little

	uint16_t cooling = ((COOLING * 10) / numLed) + 2;
	for (uint16_t i = 0; i < numLed; i++)
	{
		heat[i] = qsub8(heat[i], random8(0, cooling));
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (uint16_t k = numLed - 1; k >= 2; k--)
	{
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if (random8() < SPARKING)
	{
		int y   = random8(7);
		heat[y] = qadd8(heat[y], random8(160, 255));
	}

	// Step 4.  Map from heat cells to LED colors
	for (uint16_t j = 0; j < numLed; j++)
	{
		for (uint16_t n = 0; n < sizeX; n++)
		{
			CRGB     rgb = HeatColor(heat[j]);
			uint16_t x   = posX * sizeX + n;
			uint16_t y   = j;
			GFX()->drawPixel(x, GFX()->height() - 1 - y, GFXExtension::Color565(rgb.r, rgb.g, rgb.b));
		}
	}
}

///////////////////////////////////////////////////////////////

void CFire::Show()
{
	// Add entropy to random number generator; we use a lot of it.
	//random16_add_entropy( random());

	// Array of temperature readings at each simulation cell

	uint32_t exittime = millis() + GetShowTime();

	uint16_t countX = GFX()->width();
	if (countX >= 32)
	{
		countX = GFX()->width() / 8;
	}

	uint16_t sizeX  = GFX()->width() / countX;
	uint16_t sizeY  = GFX()->height();

	uint16_t numLed = countX*sizeY;
	byte     heat[numLed];

	uint32_t delayTime = TimeToDelayVertical(800);

	while (millis() < exittime)
	{
		for (uint16_t ofs = 0; ofs < countX; ofs++)
		{
			CalcFire(&heat[ofs * sizeY], ofs, sizeX, sizeY);
		}

		if (!Delay(delayTime))
			return;
	}
}
