// ======== Library initialization ========

#include <Arduino.h>
#include "config.h"
#include "helpers.h"
#include <Adafruit_NeoPixel.h>
using namespace std;

// ======== Setup ========

void setup() {

  // Initialize strand LED
  ledcSetup(LEDC_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(PWM_LED_PIN, LEDC_CHANNEL);

  // Setting the interrupt pin!!
  attachInterrupt(digitalPinToInterrupt(Button_Pin), buttonISR, FALLING); 

  // Initialize button
  pinMode(Button_Pin, INPUT_PULLUP);

  // Initializing the status light, will start with a green light
  StatusLight.begin();
  StatusLight.setPixelColor(0, StatusLight.Color(10, 0, 0)); // Dim yellow light for boot verification (GRB)
  StatusLight.show(); 

  Left_NeoPixel_Chain.begin();
  Left_NeoPixel_Chain.clear();
  Left_NeoPixel_Chain.show();

  Center_NeoPixel_Chain.begin();
  Center_NeoPixel_Chain.clear();
  Center_NeoPixel_Chain.show();

  Right_NeoPixel_Chain.begin();
  Right_NeoPixel_Chain.clear();
  Right_NeoPixel_Chain.show();

  
  
  // Turn on serial monitor
  Serial.begin(115200);
  Serial.println("System initializing...");

  setPattern(); 
}

// ======== Loop ========
void loop() {
  if (lampSleep) {
    setPattern();
  }
  static int lastPressCount = 0;
  if (buttonInterrupt) {
    buttonInterrupt = false;  // Reset flag

    setPattern();
  }


}

