#include <TimerOne.h>

#define outPin  10
#define delayPin  11    // this pin is just used to test the compute time

const float pi = 3.14159 ;
const float T = 50 ;    // sample time in microseconds
const float freq = 100 ;  // frequency of tone in hertz
const float omega = 2*pi*freq ;
const float A = 490 ;  // amplitude
// next line initializes oscillation with amplitude A
float a[]={0.0, A*sin(omega*T/1000000.0),0.0}; 
// c1 is the difference equation coefficient
const float c1 = (8.0 - 2.0*pow(omega*T/1000000.0,2))/(4.0+pow(omega*T/1000000.0,2));


void setup()                 
{
  Timer1.initialize(T);  // set sample time for discrete tone signal
  Timer1.pwm(outPin, 0,T);
  Timer1.attachInterrupt(compute);
  pinMode(delayPin, OUTPUT);
}

void loop()                   
{
  
}

void compute()
{
  digitalWrite(delayPin, HIGH);
  a[2] = c1*a[1] - a[0] ;  // recursion equation
  a[0] = a[1] ;
  a[1] = a[2] ; 
  Timer1.setPwmDuty(outPin, int(a[2])+512);
  digitalWrite(delayPin, LOW); 
}
