To run (and compile) all the sketches, please:

* Set Sketchbook location (Arduino Menu: File:Preferences...) to this folder
* Install additional libraries:
  * Adafruit_GFX Library (by Adafruit) - with dependencies, e.g. Adafruit BusIO
  * FastLED (by Daniel Garcia, Version >= 3.6.0)
  * SmartMatrix (by Louis Beaudoin, Version 4.0.3) 


Sketch:  

* Led150 & Led200: Led chain with WS2801 (Arduino 2560)
* Led128x32: esp32 connected to Hub75  
  Please note: using a Led-Panel with > 128 cols (e.g. 256) doesn't work with compiler version >=2, please install 1.0.6