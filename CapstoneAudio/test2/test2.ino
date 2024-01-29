#include <TimerOne.h>
#include <math.h>

#define outPin  10
#define delayPin  11
const int analogPin = A0;  // Analog pin for reading potentiometer

const float pi = 3.14159;
float T = 100;  // Initialize sample time in microseconds
float freq = 1000;  // Initialize frequency of tone in hertz
float omega = 2*pi*freq;
float A = 490;  // Amplitude
// next line initializes oscillation with amplitude A
float a[] = {0.0, A*sin(omega*T/1000000.0), 0.0};
// c1 is the difference equation coefficient
float c1 = (8.0-2.0*pow(omega*T/1000000.0, 2))/(4.0+pow(omega*T/1000000.0, 2));
float voltage;  // Variable to store calculated voltage

void setup() {
  Timer1.initialize(T);
  Timer1.pwm(outPin, 0, T);
  Timer1.attachInterrupt(compute);
  pinMode(delayPin, OUTPUT);
  pinMode(analogPin, INPUT);
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  int analogValue = analogRead(analogPin);  // Read the analog value
  voltage = (analogValue / 1023.0) * 5.0;  // Convert to voltage
  freq = 100 * pow((2000.0 / 100.0), (voltage / 5.0)); // Logarithmic, continuous voltage mapping to frequency range with floating-point precision, 100 Hz to 2000 Hz
  omega = 2 * pi * freq;  // Recalculate omega
  c1 = (8.0 - 2.0 * pow(omega * T / 1000000.0, 2)) / (4.0 + pow(omega * T / 1000000.0, 2));  // Recalculate c1
  
  Serial.print("Voltage at middle terminal: ");
  Serial.print(voltage);
  Serial.print(" V, Frequency: ");
  Serial.print(freq);
  Serial.println(" Hz");
  
  delay(500);  // Wait for 500 milliseconds before the next reading
}

void compute() {
  digitalWrite(delayPin, HIGH);
  a[2] = c1 * a[1] - a[0];
  a[0] = a[1];
  a[1] = a[2];
  Timer1.setPwmDuty(outPin, int(a[2]) + 512);
  digitalWrite(delayPin, LOW);
}
