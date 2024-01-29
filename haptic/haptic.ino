// Define the pin numbers
#define sliderPin A2 // Potentiometer connected to analog pin A2 & A3
#define knobPin A3
#define sliderOut 3     // PWM output on digital pin 3 & 5
#define knobOut 5

void setup() {
  // Initialize the serial communication (optional, for debugging)
  Serial.begin(9600);

  // Initialize the PWM pin as an output
  pinMode(sliderOut, OUTPUT);
  pinMode(knobOut, OUTPUT);
}

void loop() {
  /* Pot and Haptic Motor */
    int sliderValue = analogRead(sliderPin); // Read the value from the potentiometer
    int knobValue = analogRead(knobPin);
    int pwmSliderValue = map(sliderValue, 0, 1023, 0, 255); // Map the pot value (0-1023) to PWM range (0-255)
    int pwmKnobValue = map(knobValue, 0, 1023, 0, 255);
    analogWrite(sliderOut, pwmSliderValue); // Output the PWM signal
    analogWrite(knobOut, pwmKnobValue);
    
    // Optional: print the values for debugging
    Serial.print("Potentiometer: ");
    Serial.print(knobValue);
    Serial.print("\t PWM: ");
    Serial.println(pwmKnobValue); // change to either slider or knob

  // Short delay before the next loop iteration
  delay(100);
}
