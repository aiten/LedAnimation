#pragma once

#include <Adafruit_GFX.h>
#include <FastLED.h>

class FastLED_Matrix : public Adafruit_GFX
{
  private:

    uint16_t *_fb;
    CRGB * _leds;

  public:

    FastLED_Matrix(CRGB *leds, uint16_t *displayColors, uint16_t w, uint16_t h) :
      Adafruit_GFX(w, h)
    {
      _fb   = displayColors;
      _leds = leds;
    }

    virtual void show()
    {
      FastLED.show();
    }

    int XY(int16_t x, int16_t y)
    {
      if ((x < 0) || (y < 0) || (x >= width()) || (y >= height()))
      {
        return 0;
      }

      int16_t t;
      switch (rotation) {
        case 1:
          t = x;
          x = WIDTH  - 1 - y;
          y = t;
          break;
        case 2:
          x = WIDTH  - 1 - x;
          y = HEIGHT - 1 - y;
          break;
        case 3:
          t = x;
          x = y;
          y = HEIGHT - 1 - t;
          break;
      }

      // now x and y are raw
      
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;

      if (x % 2)
      {
        return (((x + 1) * HEIGHT) - 1 - y);
      }
      return ((x * HEIGHT) + y);
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
      if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

      auto ledIdx = XY(x, y);
      _fb[ledIdx] = color;
      _leds[ledIdx] = GFXExtension::Expand565To32(color);
    }

    void fillScreen(uint16_t color)
    {
      uint16_t numpix = _width * _height;
      auto expandCol = GFXExtension::Expand565To32(color);

      for (uint32_t i = 0; i < numpix; i++)
      {
        _fb[i] = color;
        _leds[i] = expandCol;
      }
    }
};
