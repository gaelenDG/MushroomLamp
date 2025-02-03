// ======== Includes ========

#include "helpers.h"
#include "config.h"
#include <Arduino.h>

// ======== Helper functions ========

void handleButtonPress();
void cyclePatterns();
void setPattern();
void cyclePatterns();
void enterDeepSleep();
void lightPixel(int position, int Red, int Green, int Blue, int White);

// Observe and perform button press actions
// --- Either a short press, where push for switching pattern
// --- Or a long press, where enter deep sleep
void handleButtonPress() {
  if (digitalRead(Button_Pin) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
      buttonPressTime = millis();
    }
  } else if (buttonPressed) {
    unsigned long pressDuration = millis() - buttonPressTime;
    buttonPressed = false;

    if (pressDuration >= longPressDuration) {
      enterDeepSleep();
    } else {
      cyclePatterns();
    }
  }
}

// Go to next pattern in cycle (four total, but can change)
void cyclePatterns() {
  patternIndex = (patternIndex % 4) + 1; // Cycle through 1-4
  setPattern(patternIndex);
}

// Deep sleep
// !! Will need to add functionality for waking up!!
void enterDeepSleep() {
  StatusLight.setPixelColor(0, StatusLight.Color(0, 0, 10)); // Dim blue light before sleep
  StatusLight.show();
  delay(500);
  esp_deep_sleep_start(); // Enter deep sleep
}

// Applies the appropriate pattern based on how many short button presses
void setPattern(int patternIndex) {
  // Example light effects for each pattern
  switch (patternIndex) {
    case 1:
      TestPattern();

      break;
    case 2:
      // Example: Turn on only Left chain
      Left_NeoPixel_Chain.fill(Left_NeoPixel_Chain.Color(0, 255, 0));
      Left_NeoPixel_Chain.show();
      break;
    case 3:
      // Example: Turn on only Right chain
      Right_NeoPixel_Chain.fill(Right_NeoPixel_Chain.Color(0, 0, 255));
      Right_NeoPixel_Chain.show();
      break;
    case 4:
      // Example: Turn all chains to white
      Center_NeoPixel_Chain.fill(Center_NeoPixel_Chain.Color(255, 255, 255));
      Left_NeoPixel_Chain.fill(Left_NeoPixel_Chain.Color(255, 255, 255));
      Right_NeoPixel_Chain.fill(Right_NeoPixel_Chain.Color(255, 255, 255));
      Center_NeoPixel_Chain.show();
      Left_NeoPixel_Chain.show();
      Right_NeoPixel_Chain.show();
      break;
  }
}

// Lights a specified pixel with the input color pattern 
void lightPixel(int position, int Red, int Green, int Blue, int White) {

  // Determine which chain and pixel to light up
  Adafruit_NeoPixel* chain = nullptr; // Pointer to the correct chain, once determined
  int localPosition = 0; // Pixel index within the selected chain
  uint32_t color = 0;  // Store the calculated color

  if (position >= 0 && position < 2){
    chain = &Left_NeoPixel_Chain; // left chain
    localPosition = position;
     color = chain->Color(Red, Green, Blue); // RGB pixel color assignment

  } else if (position >= 2 && position < 5) {
    chain = &Center_NeoPixel_Chain; // center chain
    localPosition = position - 2; // adjusting position for now being in the 2, 3, or 4 spot
    color = chain->Color(Red, Green, Blue, White); // RGB pixel color assignment

  } else if (position >= 5 && position <= 7) {
    chain = &Right_NeoPixel_Chain;
    localPosition = position - 5; // adjusting position for now being in the 5, 6, 0r 7 spot
    color = chain->Color(Red, Green, Blue); // RGB pixel color assignment
  }

  if (chain) { // if a valid chain is selected
    chain->setPixelColor(localPosition, color);
    chain->show();
  } else {
    // Handle invalid positions
    Serial.print("Invalid position: ");
    Serial.println(position);
  }
}

