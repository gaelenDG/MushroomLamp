// ======== Library initialization ========
// Include necessary libraries for hardware control
#include <Arduino.h>                 // Core library for ESP32
#include "config.h"                  // Configuration file with pin definitions
#include "helpers.h"                 // Helper functions for LED patterns
#include <Adafruit_NeoPixel.h>        // Library for controlling NeoPixel LEDs
using namespace std;                  // Standard namespace

// ======== Setup ========
/*
 * The setup() function initializes all hardware components, including:
 * - NeoPixel LED chains
 * - Capacitive touch sensor with an interrupt
 * - A PWM-controlled LED strand
 * - Serial communication for debugging
 * The function also sets a startup status light.
 */
void setup() {

  // Initialize LED strand with PWM settings
  ledcSetup(LEDC_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(PWM_LED_PIN, LEDC_CHANNEL);

  // Attach an interrupt to the capacitive touch button
  attachInterrupt(digitalPinToInterrupt(Button_Pin), buttonISR, FALLING); 

  // Configure the touch button pin with an internal pull-up resistor
  pinMode(Button_Pin, INPUT_PULLUP);

  // Initialize the status light with a dim yellow color for boot verification
  StatusLight.begin();
  StatusLight.setPixelColor(0, StatusLight.Color(10, 10, 0)); // Red component (GRB format)
  StatusLight.show(); 

  // Initialize and clear the left NeoPixel chain
  Left_NeoPixel_Chain.begin();
  Left_NeoPixel_Chain.clear();
  Left_NeoPixel_Chain.show();

  // Initialize and clear the center NeoPixel chain
  Center_NeoPixel_Chain.begin();
  Center_NeoPixel_Chain.clear();
  Center_NeoPixel_Chain.show();

  // Initialize and clear the right NeoPixel chain
  Right_NeoPixel_Chain.begin();
  Right_NeoPixel_Chain.clear();
  Right_NeoPixel_Chain.show();

  // Start serial communication for debugging
  Serial.begin(115200);
  Serial.println("System initializing...");

  // Set the initial LED pattern
  setPattern(); 
}

// ======== Main Loop ========
/*
 * The loop() function handles button interactions and LED behavior.
 * - If the lamp is in sleep mode, reset the pattern.
 * - If a button press is detected (interrupt flag is set), process it.
 */
void loop() {
  if (lampSleep) {
    setPattern();  // Reactivate the lamp if woken from sleep mode
  }

  static int lastPressCount = 0;  // Stores last button press count to track changes
  if (buttonInterrupt) { 
    buttonInterrupt = false;  // Reset the interrupt flag

    setPattern();  // Update LED pattern based on the button press count
  }
}
