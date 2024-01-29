#include <FastLED.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 16
#define DATA_PIN 6
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define MAX_BRIGHTNESS 120
#define FADE_RATE 1 // Adjust this value to control the fading speed

CRGB leds[NUM_LEDS];

// Global array to store the target brightness for each LED
int targetBrightness[NUM_LEDS] = {0};
int currentBrightness[NUM_LEDS] = {0};

void setup() {
    Serial.begin(9600);
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    pinMode(JOYSTICK_X, INPUT);
    pinMode(JOYSTICK_Y, INPUT);
}

void loop() {
    int x = analogRead(JOYSTICK_X) - 512; // Centering the joystick value
    int y = analogRead(JOYSTICK_Y) - 512; // Centering the joystick value
    float angle = atan2(y, x); // Calculate the angle
    int magnitude = max(abs(x), abs(y)); // Calculate how far the joystick is pushed

    // Print joystick position and calculated values
//    Serial.print("X: "); Serial.print(x);
//    Serial.print("   Y: "); Serial.print(y);
//    Serial.print("   Angle: " + String(angle) + " rad, " + String(angle * 180 / PI) + " deg");
//    Serial.print("   Magnitude: "); Serial.println(magnitude);

    // Map magnitude to brightness
    int brightness = map(magnitude, 0, 512, 0, MAX_BRIGHTNESS);
    brightness = constrain(brightness, 0, MAX_BRIGHTNESS);

    // Map angle to hue
    float angleNormalized = (angle + PI) / (2 * PI); // Normalize angle to range 0-1
    int hue = angleNormalized * 255; // Map to 0-255 range
    hue = constrain(hue, 0, 255); // Constrain to valid hue values

    // Determine LED direction based on angle with rounding
    float ledDirectionFloat = (float)((angle + PI) / (2 * PI) * NUM_LEDS);
    int ledDirection = round(ledDirectionFloat) % NUM_LEDS; // Round to the nearest integer

    // Print LED direction and brightness
//    Serial.print("LED Direction: "); Serial.print(ledDirection);
//    Serial.print("   Brightness: "); Serial.print(brightness);
//    Serial.print("   Hue: "); Serial.println(hue);

    // Set target brightness for each LED
    for (int i = 0; i < NUM_LEDS; i++) {
        int distance = min(abs(i - ledDirection), NUM_LEDS - abs(i - ledDirection));
        targetBrightness[i] = max(1, brightness - (distance * brightness / (NUM_LEDS / 2)));
//        Serial.println("LED Target Brightness [" + String(i) + "]: " + String(targetBrightness[i]));
//        Serial.println("LED distance: " + String(distance));
    }

    // Gradually adjust current brightness of LEDs towards the target brightness
    for (int i = 0; i < NUM_LEDS; i++) {
        // Move current brightness closer to the target brightness
        if (currentBrightness[i] < targetBrightness[i]) {
            currentBrightness[i] = min(currentBrightness[i] + FADE_RATE, targetBrightness[i]);
        } else if (currentBrightness[i] > targetBrightness[i]) {
            currentBrightness[i] = max(currentBrightness[i] - FADE_RATE, targetBrightness[i]);
        }
//        Serial.println("LED Current Brightness [" + String(i) + "]: " + String(currentBrightness[i]));

        // Set the LED color, saturation, and brightness
        leds[i] = CHSV(hue, 255, currentBrightness[i]);
    }

    FastLED.show();
//    Serial.print("\n");
//    delay(10); // Increased delay for readability of serial output
}
