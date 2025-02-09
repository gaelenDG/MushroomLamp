// ======== Includes ========

#include "helpers.h"
#include "config.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ======== Helper functions ========

// Observe and perform button press actions
// --- Either a short press, where push for switching pattern
// --- Or a long press, where enter deep sleep


// void handleButtonPress() {
//   if (digitalRead(Button_Pin) == LOW) {
//     if (!buttonPressed) {
//       buttonPressed = true;
//       buttonPressTime = millis();
//     }
//   } else if (buttonPressed) {
//     unsigned long pressDuration = millis() - buttonPressTime;
//     buttonPressed = false;

//     if (pressDuration >= longPressDuration) {
//       enterDeepSleep();
//     } else {
//       buttonInterrupt = true;  // Set the flag to indicate an interrupt
//     }
//   }
// }

void IRAM_ATTR buttonISR() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastPressTime > 100) {  // 100ms debounce
        if (digitalRead(Button_Pin) == LOW) {  // Button press detected
            pressStartTime = currentTime;    // Store when press started
            buttonHeld = true;               // Flag button is being held
        } 
        else if (buttonHeld) {  // Button released
            unsigned long pressDuration = currentTime - pressStartTime;
            buttonHeld = false;  // Reset flag

            if (pressDuration >= 2000) {  // Long press detected
                lampSleep = true;   // Enter default/sleep mode
                buttonPressCount = 0;
            } 
            else {  // Short press detected
                buttonPressCount = (buttonPressCount + 1) % 5;
                buttonInterrupt = true;
            }
        }
        lastPressTime = currentTime;
    }
}

// Function to check how many times the button was pressed
int getButtonPresses() {
    return buttonPressCount;
}

// Deep sleep
// !! Will need to add functionality for waking up!!
void enterDeepSleep() {
  StatusLight.setPixelColor(0, StatusLight.Color(0, 0, 10)); // Dim blue light before sleep
  StatusLight.show();
  delay(500);
  esp_deep_sleep_start(); // Enter deep sleep
}

// Function to pick a random distinct color
void pickRandomColor(int &r, int &g, int &b) {
    int index = random(numColors);
    r = colorOptions[index][0];
    g = colorOptions[index][1];
    b = colorOptions[index][2];
}

// Applies the appropriate pattern based on how many short button presses
void setPattern() {
  buttonInterrupt = false;
  patternIndex = getButtonPresses();
  // Example light effects for each pattern
  switch (patternIndex) {
    case 0:
      DefaultPattern();
      break;

    case 1:
      Pattern1();
      break;
      
    case 2:
      // Example: Turn on only Left chain
      Pattern2();
      break;
    case 3:
      // Example: Turn on only Right chain
      Pattern3();
      break;
    case 4:
      Pattern4();
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
    if (position == 0){
      localPosition = 1;
    } else {
      localPosition = 0;
    }
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

// Control the lightStrand brightness with a PWM value between 0-255
void lightStrand(int PWM) {
  ledcWrite(LEDC_CHANNEL, PWM);
}