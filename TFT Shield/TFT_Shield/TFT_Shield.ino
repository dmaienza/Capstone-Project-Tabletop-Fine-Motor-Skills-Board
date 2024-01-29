#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST77xx.h> // For ST7789 and ST7735

// For the Adafruit shield, these are the defaults.
#define TFT_CS     10
#define TFT_RST    -1  // Since the reset is connected to the Arduino reset
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello! ST7735 TFT Test");

  // Initialize TFT for green tab display
  tft.initR(INITR_BLACKTAB);   // Use this line for a black tab display

  Serial.println("Initialized");

  // Fill screen with black
  tft.fillScreen(ST77XX_BLACK);

  // Draw a red pixel in the center
  tft.drawPixel(tft.width() / 2, tft.height() / 2, ST77XX_RED);
}

void loop() {
  // Nothing to do here
}
