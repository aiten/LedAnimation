
#define GRIDX 15
#define GRIDY 10

#define NUM_LEDS (GRIDX*GRIDY)

// +5   => rot
// GND  => blau
// 52   => grün
// 51   => weiss

/*
  The circuit:
   DO NOT connect SD card to SPI bus - we use SoftwareSPI:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS  - pin 10
*/
