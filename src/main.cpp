// ======== Library initialization ========

#include <Arduino.h>
#include "config.h"
#include "helpers.h"
#include <Adafruit_NeoPixel.h>
using namespace std;

// ======== Setup ========

void setup() {

  // Initialize button
  pinMode(Button_Pin, INPUT_PULLUP);

  // Initializing the status light, will start with a green light
  StatusLight.begin();
  StatusLight.setPixelColor(0, StatusLight.Color(10, 0, 0)); // Dim yellow light for boot verification (GRB)
  StatusLight.show(); 

  // Initializing neopixel chains
  Center_NeoPixel_Chain.begin();
  Right_NeoPixel_Chain.begin();
  Left_NeoPixel_Chain.begin();
  
  // Turn on serial monitor
  Serial.begin(9600);
  Serial.println("System initializing...");

  // Start with Pattern 1
  setPattern(patternIndex);
}

// ======== Loop ========
void loop() {
  // Temporarily just applying TestPattern() here!
  TestPattern()


  // Once done testing, uncomment the following to run button behavior
  //handleButtonPress();
}

