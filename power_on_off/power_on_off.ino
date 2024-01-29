long shutdownTimer = 0;

void setup() {
  // initialize pin 13 as output 
  // to blink the on-board LED
  pinMode(13, OUTPUT);

  // initialize the button status pin
  pinMode(10, INPUT_PULLUP);

  // hold the power mosfet ON
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  // wait for the shut-down timer
  while (millis() - shutdownTimer < 5000) {
    // if the button is pressed 
    // (pin 10 is pulled to ground)
    // then extend shutdownTimer
    // and blink the on-board LED
    if (!digitalRead(10)) {
      shutdownTimer = millis();
      blinkLed();
    }
  }
  // when the time is up then shut it down
  digitalWrite(7, LOW);
  // give it time to shut down
  delay(1000);
}

void blinkLed() {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
}
