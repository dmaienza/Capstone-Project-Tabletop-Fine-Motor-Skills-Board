#include <SineWave.h>
#include <TimerOne.h>

const int analogPin = A0;  // Declare analog pin A0 for reading potentiometer
float voltage;             // Declare variable to store calculated voltage
float freq;  // Initialize frequency of tone in hertz

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud
  sw.setInterval(50);
  sw.setPin(9);
}

void loop()
{
  int analogValue = analogRead(analogPin);  // Read the analog value
  voltage = (analogValue / 1023.0) * 5.0;   // Convert to voltage
  freq = 100 * pow((2000.0 / 100.0), (voltage / 5.0)); // Logarithmic, continuous voltage mapping to frequency range with floating-point precision, 100 Hz to 2000 Hz

  Serial.print("Voltage at middle terminal: ");
  Serial.print(voltage);
  Serial.print(" V, Frequency: ");
  Serial.print(freq);
  Serial.println(" Hz");
  
  sw.playTone(freq);
  delay(500);
  sw.stopTone();
}
