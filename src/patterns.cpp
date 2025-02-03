
// ======== Includes ========

#include "helpers.h"
#include "config.h"
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;


// Test Pattern
// --- Temporary structure for building patterns, move to Patterni() as appropriate
void TestPattern() {
  int Red = 100;
  int Green = 50;
  int Blue = 0;
  int White = 0;


  for (int i = 0; i < 8; i++){
    lightPixel(i, Red, Green, Blue, White);
    delay(500);
  }
}

// ======== Pattern 1 ========
// All pixels shift smoothly from red to green to blue to red
void Pattern1() {

  // Report pattern to serial monitor
  Serial.print("Beginning Pattern1!");

  int red = 255;        // Start with full red
  int green = 0;        // No green at the start
  int blue = 0;         // No blue at the start
	int white = 0;				// No white ever

  int fadeSpeed = 1;    // Adjust this for faster/slower color transitions
  // Transition from red to green
  while (red > 0 && green < 255) {
    red -= fadeSpeed;
    green += fadeSpeed;

    // Set both pixels to the same color
    for (int i = 0; i < 3; i++) {
      Right_NeoPixel_Chain.setPixelColor(i, Right_NeoPixel_Chain.Color(red, green, blue));
    }
    // Set both pixels to the same color
    for (int i = 0; i < 2; i++) {
      Left_NeoPixel_Chain.setPixelColor(i, Left_NeoPixel_Chain.Color(red, green, blue));
    }

    Right_NeoPixel_Chain.show();
    Left_NeoPixel_Chain.show();

    delay(40);  // Delay to control transition speed
  }

  // Transition from green to blue
  while (green > 0 && blue < 255) {
    green -= fadeSpeed;
    blue += fadeSpeed;

    // Set both pixels to the same color
    for (int i = 0; i < 3; i++) {
      Right_NeoPixel_Chain.setPixelColor(i, Right_NeoPixel_Chain.Color(red, green, blue));
    }
    
    // Set both pixels to the same color
    for (int i = 0; i < 2; i++) {
      Left_NeoPixel_Chain.setPixelColor(i, Left_NeoPixel_Chain.Color(red, green, blue));
    }

    Right_NeoPixel_Chain.show();
    Left_NeoPixel_Chain.show();

    delay(40);
  }

  // Transition from blue to red
  while (blue > 0 && red < 255) {
    blue -= fadeSpeed;
    red += fadeSpeed;

    // Set both pixels to the same color
    for (int i = 0; i < 3; i++) {
      Right_NeoPixel_Chain.setPixelColor(i, Right_NeoPixel_Chain.Color(red, green, blue));
    }

    // Set both pixels to the same color
    for (int i = 0; i < 2; i++) {
      Left_NeoPixel_Chain.setPixelColor(i, Left_NeoPixel_Chain.Color(red, green, blue));
    }

    Right_NeoPixel_Chain.show();
    Left_NeoPixel_Chain.show();

    delay(40);
  }

}

