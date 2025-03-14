// ======== Includes ========
// Include necessary libraries for LED control and hardware interaction
#include "helpers.h"
#include "config.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ======== Helper Functions ========

/*
 * Interrupt Service Routine (ISR) for the capacitive touch button.
 * Detects short and long presses:
 * - Short press: Cycles through lighting patterns.
 * - Long press (≥2 seconds): Enters deep sleep mode.
 */
void IRAM_ATTR buttonISR() {
    unsigned long currentTime = millis();
    
    // Debounce to prevent rapid, unintentional triggers
    if (currentTime - lastPressTime > 100) {  
        if (digitalRead(Button_Pin) == LOW) {  // Button press detected
            pressStartTime = currentTime;    // Store when press started
            buttonHeld = true;               // Mark button as being held
        } 
        else if (buttonHeld) {  // Button released
            unsigned long pressDuration = currentTime - pressStartTime;
            buttonHeld = false;  // Reset flag

            if (pressDuration >= 2000) {  // Long press detected (2+ seconds)
                lampSleep = true;   // Enter sleep mode
                buttonPressCount = 0;  // Reset press count
            } 
            else {  // Short press detected
                buttonPressCount = (buttonPressCount + 1) % 5;  // Cycle through 5 patterns
                buttonInterrupt = true;  // Flag that a press occurred
            }
        }
        lastPressTime = currentTime;  // Update last press timestamp
    }
}

/*
 * Retrieves the current button press count.
 * Used to determine which LED pattern to display.
 * @return The number of short button presses since last reset.
 */
int getButtonPresses() {
    return buttonPressCount;
}

/*
 * Puts the ESP32 into deep sleep mode.
 * !! Future enhancement: Add functionality for wake-up !!
 */
void enterDeepSleep() {
  // Indicate sleep entry with a dim blue status light
  StatusLight.setPixelColor(0, StatusLight.Color(0, 0, 10)); 
  StatusLight.show();
  delay(500);

  esp_deep_sleep_start(); // Enter deep sleep mode
}

/*
 * Selects a random color from a predefined list.
 * @param r Reference to red value (0-255)
 * @param g Reference to green value (0-255)
 * @param b Reference to blue value (0-255)
 */
void pickRandomColor(int &r, int &g, int &b) {
    int index = random(numColors);  // Pick random index from available colors
    r = colorOptions[index][0];
    g = colorOptions[index][1];
    b = colorOptions[index][2];
}

/*
 * Determines and applies the appropriate LED pattern based on button presses.
 * - Pattern 0: Default mode
 * - Pattern 1-4: Various lighting sequences
 */
void setPattern() {
  buttonInterrupt = false;  // Reset button interrupt flag
  patternIndex = getButtonPresses();  // Get press count to determine pattern

  switch (patternIndex) {
    case 0:
      DefaultPattern();
      break;
    case 1:
      Pattern1();
      break;
    case 2:
      Pattern2();
      break;
    case 3:
      Pattern3();
      break;
    case 4:
      Pattern4();
      break;
  }
}

/*
 * Lights up a specific NeoPixel at a given position with a specified color.
 * Supports RGB (or RGBW for center chain).
 * @param position The pixel position (0-7) across all chains.
 * @param Red Red brightness (0-255)
 * @param Green Green brightness (0-255)
 * @param Blue Blue brightness (0-255)
 * @param White White brightness (0-255) (only for center chain)
 */
void lightPixel(int position, int Red, int Green, int Blue, int White) {
  Adafruit_NeoPixel* chain = nullptr;  // Pointer to correct LED chain
  int localPosition = 0;  // Pixel index within the selected chain
  uint32_t color = 0;  // Store calculated color

  if (position >= 0 && position < 2) {
    chain = &Left_NeoPixel_Chain;
    localPosition = (position == 0) ? 1 : 0;  // Swap positions for alignment
    color = chain->Color(Red, Green, Blue);  

  } else if (position >= 2 && position < 5) {
    chain = &Center_NeoPixel_Chain;
    localPosition = position - 2;  // Adjust for center chain index
    color = chain->Color(Red, Green, Blue, White);  // Center supports RGBW

  } else if (position >= 5 && position <= 7) {
    chain = &Right_NeoPixel_Chain;
    localPosition = position - 5;  // Adjust for right chain index
    color = chain->Color(Red, Green, Blue);  

  } else {
    Serial.print("Invalid position: ");
    Serial.println(position);
    return;  // Exit function if position is out of range
  }

  // If a valid chain is assigned, apply the color and update LEDs
  if (chain) {
    chain->setPixelColor(localPosition, color);
    chain->show();
  }
}

/*
 * Sets the brightness of the LED strand using PWM.
 * @param PWM A value between 0 (off) and 255 (full brightness)
 */
void lightStrand(int PWM) {
  ledcWrite(LEDC_CHANNEL, PWM);  // Apply PWM value to LED strand
}
