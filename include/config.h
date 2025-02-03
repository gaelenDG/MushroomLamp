
// ======== Includes ========

#include <Adafruit_NeoPixel.h>
#include <vector>

// ======== GPIO Pin definitions ========

// Defining GPIO pins
#define Left_NeoPixel_Pin 6
#define Right_NeoPixel_Pin 5
#define Center_NeoPixel_Pin 4
#define Center_Strand_Pin 7
#define Button_Pin 0
#define BuiltIn_NeoPixel 10

// ======== Neopixel chain definitions ========

Adafruit_NeoPixel StatusLight(1, BuiltIn_NeoPixel, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Center_NeoPixel_Chain(3, Center_NeoPixel_Pin, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel Left_NeoPixel_Chain(2, Left_NeoPixel_Pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Right_NeoPixel_Chain(3, Right_NeoPixel_Pin, NEO_GRB + NEO_KHZ800);

// ======== Constants ========

// A (very simple) map of the NeoPixels for adjacency
vector<int> graph[8] = {
  {1},
  {0, 2},
  {1, 3},
  {2, 4},
  {3, 5},
  {4, 6},
  {5, 7},
  {6, 0}
};

// Initializing the button and its behavior
int patternIndex = 1;  // Start with Pattern 1
unsigned long buttonPressTime = 0;
bool buttonPressed = false;
const int longPressDuration = 2000; // 2 seconds for long press
