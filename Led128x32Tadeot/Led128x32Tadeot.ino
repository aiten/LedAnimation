// for esp32 (Vesion 2.0.x) => https://github.com/pixelmatix/SmartMatrix/issues/171
// => copy some includes to esp32_i2s_parallel.c
// if kMatrixWidth > 128 use 1.0.6 (otherwie some memory error with 3.x)

#define USE_ADAFRUIT_GFX_LAYERS

// uncomment one line to select your MatrixHardware configuration - configuration header needs to be included before <SmartMatrix3.h>
//#include <MatrixHardware_ESP32_V0.h>    // This file contains multiple ESP32 hardware configurations, edit the file to define GPIOPINOUT (or add #define GPIOPINOUT with a hardcoded number before this #include)
//#include <MatrixHardware_KitV1.h>       // SmartMatrix Shield for Teensy 3 V1-V3
//#include <MatrixHardware_KitV4.h>       // SmartLED Shield for Teensy 3 V4
//#include <MatrixHardware_KitV4T4.h>     // Teensy 4 Wired to SmartLED Shield for Teensy 3 V4
//#include <MatrixHardware_T4Adapter.h>   // Teensy 4 Adapter attached to SmartLED Shield for Teensy 3 V4
#include "MatrixHardware_Custom.h" // Copy an existing MatrixHardware file to your Sketch directory, rename, customize, and you can include it like this
#include <SmartMatrix.h>

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint16_t kMatrixWidth = 256;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 24;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 2;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kScrollingLayerOptions = (SM_GFX_MONO_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);

const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

const int defaultBrightness = (100 * 255) / 100; // full (100%) brightness

//////////////////////////////////////////////////////////////////////////////

#include <LedAnimation.h>
#include <sample.h>

template <typename RGB, unsigned int optionFlags>
class MyGraphic : public GFXExtension
{
  SMLayerBackgroundGFX<RGB, optionFlags> *_layer;

public:
  MyGraphic(SMLayerBackgroundGFX<RGB, optionFlags> *layer) : GFXExtension(layer)
  {
    _layer = layer;
  };

protected:
  virtual void Show() override
  {
  }
  virtual uint16_t getPixel(uint16_t x, uint16_t y) override
  {
    RGB col = _layer->readPixel(x, y);
    return Color565(col.red,col.green,col.blue);
  }
};

MyGraphic<rgb24, 0> myGraphic(&backgroundLayer);

//////////////////////////////////////////////////////////////////////////////

CHelix helix;
CBall ball1(1000000,2,true,500);
CBall ball2(10000,10,false,2000,4000);
CBall ball3(100000,20,false,2000,4000);
CBall ball4(100000,30,false,2000,4000);
CBox box(10000);
CWheel wheel(10000);

CFire fire(10000);

CRotatedLine rotate;

CCounter counter1(0, 0, 199, 255, 100);
CCounter counter2(0, 198, 0, 1, 100);
CCounter counter3(0, 1, 19, 255, 1000);
CCounter counter4(0, 9, 1, 255, 2000);

CTicker tickerABC("a\344bcdefghijklmno\366pqrstu\374vwxyzA\304BCDEFGHIJKLMNO\326PQRSTU\334VWXYZ-+*.!/,?()0123456789", CTicker::ScrollFromRight);
CTicker tickerHTL("HTL Leonding", CTicker::ScrollFromRight);
CTicker robo("Roboterfuehrerschein");
CTicker tadot("Tag der offenen Tuer 2025");

CAnimation *animations[] =
    {
      
        //NULL,
        &tickerHTL,
        &robo,
        &tadot,        
        &box,
        &tickerHTL,
        &robo,
        &fire,
        &tickerHTL,
        &robo,
        &ball2,
        &tickerHTL,
        &robo,
        &wheel,
        NULL};

unsigned char animationsIdx = 0;

//////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  CAnimation::InitGlobal(&myGraphic);

  matrix.addLayer(&backgroundLayer);
  //  matrix.addLayer(&scrollingLayer);
  matrix.begin();

  matrix.setBrightness(defaultBrightness);

  backgroundLayer.enableColorCorrection(true);


  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  backgroundLayer.swapBuffers();
}

//////////////////////////////////////////////////////////////////////////////

long nextRedraw = 0;

void loop()
{
  // return;
  if (animations[animationsIdx]->IsEnabled())
  {
    animations[animationsIdx]->SetAskContinue(0, [](int) -> bool {
      if (nextRedraw < millis())
      {
        backgroundLayer.swapBuffers();
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
