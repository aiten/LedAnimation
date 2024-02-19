#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <SPI.h>
#include <SDSoftwareSPI.h>
#include <LedAnimation.h>

#include <Sample.h>

#include "FastLED_Matrix.h"
#include "Led.h"

//////////////////////////////////////////////////////////////////////////////

CRGB leds[NUM_LEDS];
uint16_t displayColor[NUM_LEDS];

FastLED_Matrix matrix(leds, displayColor, GRIDX, GRIDY);

class MyGraphic : public GFXExtension
{
    FastLED_Matrix* _layer;

  public:
    MyGraphic(FastLED_Matrix* layer) : GFXExtension(layer)
    {
      _layer = layer;
    };

  protected:
    virtual void Show() override
    {
      _layer->show();
    }
    virtual uint16_t getPixel(uint16_t x, uint16_t y) override
    {
      return displayColor[_layer->XY(x, y)];
    }
};

MyGraphic myGraphic(&matrix);

//////////////////////////////////////////////////////////////////////////////

CHelix helix;
CBall ball1(180000, 1, true, 1000);
CBall ball2(180000, 2, false, 2000, 1000);
CBall ball3(100000, 20, false, 2000, 1000);
CBall ball4(100000, 30, false, 2000, 1000);
CBox box(5000);
CWheel wheel(14000);

CRotatedLine rotate;

CFire fire(60000);

CCounter counter1(0, 0, 199, 255, 100);
CCounter counter2(0, 198, 0, 1, 100);
CCounter counter3(0, 1, 19, 255, 1000);
CCounter counter4(0, 9, 1, 255, 2000);

CTicker tickerABC("a\344bcdefghijklmno\366pqrstu\374vwxyzA\304BCDEFGHIJKLMNO\326PQRSTU\334VWXYZ-+*.!/,?()0123456789", CTicker::ScrollFromRight);
CTicker tickerHTL("HTL");
CTicker ticker1("Le", CTicker::ScrollSingleCharFadeInRightFadeOutRight);
CTicker ticker2("on", CTicker::ScrollSingleCharFadeInRightFadeOutLeft);
CTicker ticker3("di", CTicker::ScrollSingleCharFadeInRightFadeOutUp);
CTicker ticker4("ng", CTicker::ScrollSingleCharFadeInRightFadeOutDown);
CTicker tickerCR("by HTL Leonding 2022", CTicker::ScrollFromRight, 1000);

CAnimation *animations[] =
{
  // &testanimation,
  //&tickerABC,
  &tickerHTL,
  &ticker1,
  &ticker2,
  &ticker3,
  &ticker4,
  &tickerCR,
  &fire,
  &box,
  &wheel,
  &rotate,
  &counter1,
  &counter2,
  &counter3,
  &counter4,
  &ball1,
  &ball2,
  //&ball3,
  //&ball4,
  &helix,
  //&sdTicker,
  //&tickerCopyRight,
  NULL
};

unsigned char animationsIdx = 0;

//////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  CAnimation::InitGlobal(&myGraphic);

  FastLED.addLeds<LPD6803, 51, 52, RGB, DATA_RATE_MHZ(4)>(leds, NUM_LEDS);
  //        FastLED.addLeds<WS2801, 51,52, RGB, DATA_RATE_MHZ(8)>(leds, NUM_LEDS);  => only with 150Leds
  //        FastLED.addLeds<WS2801, 51,52, RGB>(leds, NUM_LEDS);
  //        FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

//////////////////////////////////////////////////////////////////////////////

unsigned long nextRedraw = 0;

void loop()
{
  // return;
  if (animations[animationsIdx]->IsEnabled())
  {
    animations[animationsIdx]->SetAskContinue(0, [](int) -> bool {
      if (nextRedraw < millis())
      {
        //backgroundLayer.swapBuffers();
        matrix.show();
        nextRedraw = millis() + 1000 / 25;
      }
      return true;
    });

    Serial.println("show");
    animations[animationsIdx]->Show();
  }

  animationsIdx++;

  Serial.println("next");

  if (animations[animationsIdx] == 0)
  {
    animationsIdx = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////
