#define USE_ADAFRUIT_GFX_LAYERS

// uncomment one line to select your MatrixHardware configuration - configuration header needs to be included before <SmartMatrix3.h>
//#include <MatrixHardware_ESP32_V0.h>    // This file contains multiple ESP32 hardware configurations, edit the file to define GPIOPINOUT (or add #define GPIOPINOUT with a hardcoded number before this #include)
//#include <MatrixHardware_KitV1.h>       // SmartMatrix Shield for Teensy 3 V1-V3
//#include <MatrixHardware_KitV4.h>       // SmartLED Shield for Teensy 3 V4
//#include <MatrixHardware_KitV4T4.h>     // Teensy 4 Wired to SmartLED Shield for Teensy 3 V4
//#include <MatrixHardware_T4Adapter.h>   // Teensy 4 Adapter attached to SmartLED Shield for Teensy 3 V4
#include "MatrixHardware_Custom.h"      // Copy an existing MatrixHardware file to your Sketch directory, rename, customize, and you can include it like this
#include <SmartMatrix.h>

#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSerif24pt7b.h>
#include <Fonts/FreeMono24pt7b.h>

#include <Fonts/FreeMono18pt7b.h>

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint16_t kMatrixWidth = 128;      // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 24;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kScrollingLayerOptions = (SM_GFX_MONO_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);

const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

const int defaultBrightness = (100 * 255) / 100;    // full (100%) brightness
const int defaultScrollOffset = 6;

// --------------------------------------------------------

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "A1";
const char* password = "xxxx";

WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// --------------------------------------------------------

long starttime = 0;
long lastSec = 0;

void setup() {

  Serial.begin(115200);

  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer);
  matrix.begin();

  matrix.setBrightness(defaultBrightness);

  scrollingLayer.setOffsetFromTop(defaultScrollOffset);

  backgroundLayer.enableColorCorrection(true);

  // ----------------

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {

    for (uint8_t i = 0; i < server.args(); i++) {
      Serial.print(server.argName(i));
      Serial.print(": ");
      Serial.println(server.arg(i));

      scrollingLayer.setFont(font8x13);
      scrollingLayer.setColor({0xff, 0, 0});
      scrollingLayer.setMode(wrapForward);
      scrollingLayer.setSpeed(40);

      static char text[256];
      strcpy(text, server.arg(i).c_str());

      scrollingLayer.start(text, 1);
    }

    server.send(200, "text/plain", "this works as well");
  });

  server.on("/start", []() {

    starttime = millis();

    server.send(200, "text/plain", "time started");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  // --------------------------------

}

int r = 0;
int g = 0;
int b = 0;

int dr = 1;
int dg = 1;
int db = 1;

int x = 0;
int y = 0;
int dx = 1;
int dy = 1;

long dxnext = 0;
long dxnext1 = 0;

int next(int v, int&d, int maxV)
{
  v += d;
  if (v > maxV || v < 0)
  {
    d = -d;
    v += 2 * d;
  }
  return v;
}

long toSec(long ticks)
{
  return (ticks - starttime) / 1000;
}

word ConvertRGB( byte R, byte G, byte B)
{
  return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );
}

void loop() {

  int speed1 = 1000/20;
  int speed2 = 1000/20;

  bool needswapBuffer = false;

  server.handleClient();

  auto now = millis();

  if (dxnext + speed1 < now)
  {
    dxnext = now;
    backgroundLayer.drawPixel(0, 0, ConvertRGB(r, r, r));

    backgroundLayer.drawPixel(63, 0, ConvertRGB(r, 0, 0));
    backgroundLayer.drawPixel(64, 0, ConvertRGB(0, r, 0));
    backgroundLayer.drawPixel(127, 0, ConvertRGB(0, 0, r));
    backgroundLayer.drawPixel(127, 31, ConvertRGB(0, r, r));
    backgroundLayer.drawPixel(64, 31, ConvertRGB(r, 0, r));
    backgroundLayer.drawPixel(63, 31, ConvertRGB(r, r, 0));
    backgroundLayer.drawPixel(0, 31, ConvertRGB(r, r / 2, 0));
    needswapBuffer = true;

    r = next(r, dr, 255);
  }

  if (dxnext1 + speed2 < now)
  {
    dxnext1 = now;

    backgroundLayer.drawPixel(x, y, ConvertRGB(0, 0, 0));

    x = next(x, dx, matrix.getScreenWidth() - 1);
    y = next(y, dy, matrix.getScreenHeight() - 1);

    backgroundLayer.drawPixel(x, y, ConvertRGB( 255, 255, 0));
    needswapBuffer = true;
  }

  auto newSec = toSec(now);
  auto oldSec = toSec(lastSec);

  if (newSec != oldSec)
  {
    lastSec = now;

    char text[128];
    backgroundLayer.setFont(&FreeMono24pt7b);
    //backgroundLayer.setBitmapFont((bitmap_font *)&calibri21x28b);
    sprintf(text, "%d:%02d", (int) (newSec / 60), (int)(newSec % 60));

    backgroundLayer.fillRect(1, 1, 126, 30, ConvertRGB( 0, 0, 16));

    int x0 = 0;
    int y0 = 29;

    int dx = -2;
    for (int i = 0; text[i]; i++)
    {
      int xshift = 0;
      int yshift = 0;
      int xadd = 20;

      if (text[i] == ':')
      {
        xshift = -4;
        xadd = 12;
        yshift = -4;
      }

      backgroundLayer.drawChar(dx + xshift, y0 + yshift, text[i], ConvertRGB( 0xff, 0xff, 0xff), 0, 1);
      dx += xadd;
    }

    needswapBuffer = true;
  }

  if (needswapBuffer)
  {
    backgroundLayer.swapBuffers();
  }
}
