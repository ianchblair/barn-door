/*
  barn_door
  Ardiono code for barn door tracker (for Thingiverse 43213177)
  This version 2020-10-19.

  Note: stepper.h not used as A4988 driver chip does most of the donkey work
*/

// MEMA 17 Motor is 1.8 degrees per pulse (200 pulses per turn)
#define SLOW_DELAY 3997   // Time per pulse in microseconds when tracking 3997 = 1.09 turns per minute for the “engranaje grande”
#define FAST_DELAY 250    // Time per pulse during adjustment (eight times faster)
 
// Pin assignments
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int led = 13;
const int nforward_back = 8;  // Direction pin
const int step_pulse = 9;     // Stepper pulse pin
const int nfast_forward = 6;  // fast forward switch (active low)
const int nfast_backward = 7; // fast backward switch (active low)
int ledState = LOW;
unsigned long previousMillis = 0;
const long led_interval = 500; // the setup routine runs once when you press reset:


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);   
  pinMode(step_pulse, OUTPUT);
  pinMode(nforward_back, OUTPUT);
  pinMode(nfast_forward, INPUT_PULLUP);
  pinMode(nfast_backward, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  long step_interval = FAST_DELAY; 
  unsigned long currentMillis = millis();

  digitalWrite(step_pulse, LOW); 
  digitalWrite(step_pulse, HIGH);
  if (digitalRead(nfast_forward) == LOW)      // Here for fast forwards adjustment
  {
    digitalWrite(nforward_back, LOW);
    step_interval = FAST_DELAY;
    ledState = HIGH;                           // Force LED on continuously when adjusting
    digitalWrite(led, ledState);
  }
  else if (digitalRead(nfast_backward) == LOW) // Here for fast backwards adjustment 
  {
    digitalWrite(nforward_back, HIGH);    
    step_interval = FAST_DELAY;
    ledState = HIGH;                           // Force LED on continuously when adjusting
    digitalWrite(led, ledState);
  }
  else                                         // Here when tracking normally
  {
    digitalWrite(nforward_back, LOW);    
    step_interval = SLOW_DELAY;
    // Flash LED at led_interval  when running normally 
    if(currentMillis - previousMillis >= led_interval) 
    { // save the last time you blinked the LED
      previousMillis = currentMillis; // if the LED is off turn it on and vice-versa: 
      if (ledState == LOW) ledState = HIGH; else ledState = LOW; // set the LED with the ledState of the variable:
      digitalWrite(led, ledState);
    }
  }
  delayMicroseconds(step_interval); 
}
