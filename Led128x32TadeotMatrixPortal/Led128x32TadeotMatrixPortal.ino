#include <Adafruit_Protomatter.h>

// https://learn.adafruit.com/adafruit-protomatter-rgb-matrix-library/arduino-library
// https://github.com/adafruit/Adafruit_Protomatter
// Board:Adafruit MatrixPortal ESP32-S3

//////////////////////////////////////////////////////////////////////////////

#include <LedAnimation.h>
#include <sample.h>

#define ARDUINO_ADAFRUIT_MATRIXPORTAL_ESP32S3
//#define HA_MATRIX_ADAPTER_ESP32

#if defined(ARDUINO_ADAFRUIT_MATRIXPORTAL_ESP32S3) // MatrixPortal ESP32-S3

const int FPS = 25;
const int BITDEPTH = 5;
const bool DOUBLEBUFFER = true;

uint8_t rgbPins[]  = {42, 41, 40, 38, 39, 37};
uint8_t addrPins[] = {45, 36, 48, 35, 21};
uint8_t clockPin   = 2;
uint8_t latchPin   = 47;
uint8_t oePin      = 14;

#elif defined(HA_MATRIX_ADAPTER_ESP32) // HA private build HUB75 MatrixAdapter ESP32-S3

const int FPS = 10;
const int BITDEPTH = 4;
const bool DOUBLEBUFFER = false;

uint8_t rgbPins[]  = {2, 15, 4, 16, 27, 17};
uint8_t addrPins[] = {5, 18, 19, 21, 12};
uint8_t clockPin   = 22;
uint8_t latchPin   = 26;
uint8_t oePin      = 25;

#endif

Adafruit_Protomatter matrix(
  256,         // Width of matrix (or matrix chain) in pixels
  BITDEPTH,    // Bit depth, 1-6
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  DOUBLEBUFFER);      // No double-buffering here (see "doublebuffer" example)


class MyGraphic : public GFXExtension
{

  Adafruit_Protomatter* _gfx;
  long _nextRedraw = 0;


public:
  MyGraphic(Adafruit_Protomatter* gfx) : GFXExtension(gfx)
  {
    _gfx = gfx;
  };

protected:
  virtual void Show(bool force) override
  {
    if (force || _nextRedraw < millis())
    {
      _nextRedraw = millis() + (1000 / FPS);
      _gfx->show();
    }
  }
  virtual uint16_t getPixel(uint16_t x, uint16_t y) override
  {
    return _gfx->getPixel(x, y);
  }
};

MyGraphic myGraphic(&matrix);


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

  //pinMode(LED_BUILTIN,OUTPUT);

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if(status != PROTOMATTER_OK) {
    for(;;);
  }

  CAnimation::InitGlobal(&myGraphic);
}

//////////////////////////////////////////////////////////////////////////////

long nextEventTime = 0;

void loop()
{
  if (animations[animationsIdx]->IsEnabled())
  {
    
    animations[animationsIdx]->SetAskContinue(0, [](int) -> bool {
      if (nextEventTime < millis())
      {
        nextEventTime = millis() + 1000 / 25;
      }
      return true;
    });
  
    Serial.println("show");
    animations[animationsIdx]->Show();
  }

  //digitalWrite(LED_BUILTIN,digitalRead(LED_BUILTIN)!);

  animationsIdx++;

  Serial.println("next");

  if (animations[animationsIdx] == 0)
  {
    animationsIdx = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////
