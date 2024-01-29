#include <FastLED.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 16
#define DATA_PIN 6
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define MAX_BRIGHTNESS 120

CRGB leds[NUM_LEDS];

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
    Serial.print("X: "); Serial.print(x);
    Serial.print("   Y: "); Serial.print(y);
    Serial.print("   Angle: "); Serial.print(String(angle) + ", " + String(angle * 180 / PI));
    Serial.print("   Magnitude: "); Serial.println(magnitude);

    // Map magnitude to brightness
    int brightness = map(magnitude, 0, 512, 0, MAX_BRIGHTNESS);
    brightness = constrain(brightness, 0, MAX_BRIGHTNESS);

    // Determine LED direction based on angle with rounding
    float ledDirectionFloat = (float)((angle + PI) / (2 * PI) * NUM_LEDS);
    int ledDirection = round(ledDirectionFloat) % NUM_LEDS; // Round to the nearest integer

    // Print LED direction and brightness
    Serial.print("LED Direction: "); Serial.print(ledDirection);
    Serial.print("   Brightness: "); Serial.println(brightness);

    // Set LED colors and brightness
    for (int i = 0; i < NUM_LEDS; i++) {
        int distance = min(abs(i - ledDirection), NUM_LEDS - abs(i - ledDirection));
        int ledBrightness = max(1, brightness - (distance * brightness/(NUM_LEDS/2))); // Decrease brightness based on distance from the direction
        Serial.println("LED Brightness [" + String(i) + "]: " + String(ledBrightness));
        Serial.println("LED distance: " + String(distance));
        leds[i] = CHSV(0, 0, ledBrightness); // Adjust the hue and saturation as needed
    }

    FastLED.show();
    Serial.print("\n");
    delay(1000); // Increased delay for readability of serial output
}
