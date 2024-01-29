#include <FastLED.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 80
#define DATA_PIN 5
#define BUTTON_PIN 7
#define BRIGHTNESS 64
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
const CRGBPalette16 blackPalette(CRGB::Black); // Define an all-black palette

int currentMode = 0;
bool lastButtonState = LOW;
float motionSpeed = 0.2; // Adjust this value to control the speed

void setup() {
    Serial.begin(9600);
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(  BRIGHTNESS );

    pinMode(BUTTON_PIN, INPUT_PULLUP); // Assuming an active-low button

    currentPalette = blackPalette;
    currentBlending = LINEARBLEND;
}

void loop() {
    // Button press detection
    bool buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == LOW && lastButtonState == HIGH) {
        currentMode++;
        changePaletteBasedOnMode();
        delay(50); // Debounce delay
    }
    lastButtonState = buttonState;

    static float accumulator = 0.0;
    static uint8_t startIndex = 0;
    accumulator += motionSpeed;
    if (accumulator >= 1.0) {
        startIndex = startIndex + 1; // motion speed
        accumulator -= 1.0; // Reset the accumulator
    }
    
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();

}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    //uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
        colorIndex += 3;
    }
}

// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
void changePaletteBasedOnMode() {
    switch (currentMode) {
        case 0: currentPalette = blackPalette;             currentBlending = LINEARBLEND; break; // Case for turning off the LEDs
        case 1: currentPalette = RainbowColors_p;          currentBlending = LINEARBLEND; break;
        case 2: currentPalette = RainbowStripeColors_p;    currentBlending = LINEARBLEND; break;
        case 3: currentPalette = CloudColors_p;            currentBlending = LINEARBLEND; break;
        case 4: currentPalette = PartyColors_p;            currentBlending = LINEARBLEND; break;
        case 5: currentPalette = OceanColors_p;            currentBlending = LINEARBLEND; break;
        case 6: currentPalette = LavaColors_p;             currentBlending = LINEARBLEND; break;
        case 7: currentPalette = ForestColors_p;           currentBlending = LINEARBLEND; break;
        case 8: currentPalette = HeatColors_p;             currentBlending = LINEARBLEND; break;
        case 9: SetupPurpleAndGreenPalette();              currentBlending = LINEARBLEND; break;
        case 10: SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; break;
        case 11: SetupRedAndBlueStripedPalette();          currentBlending = LINEARBLEND; break;
        case 12: currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; break;


        // Add more cases for each mode
        // Reset to first mode if exceeded
        default: currentMode = 0; changePaletteBasedOnMode(); break;
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupRedAndBlueStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Purple);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Red;
    currentPalette[4] = CRGB::Blue;
    currentPalette[8] = CRGB::Red;
    currentPalette[12] = CRGB::Blue; 
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
