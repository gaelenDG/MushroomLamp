// ======== Includes ========

#include <Adafruit_NeoPixel.h>
#include <vector>
#include "config.h"
using namespace std;

/*
 * Includes the Adafruit NeoPixel library for LED control.
 * Uses the vector library for data structures.
 * Includes "config.h" for additional configuration settings.
 */

// ======== GPIO Pin Definitions ========
const int Button_Pin = 0;  // GPIO pin for the capacitive touch button.
const int LED_Strand = 7;  // GPIO pin for the LED strand.

// ======== NeoPixel Definitions ========
/*
 * Defines different NeoPixel objects with their respective pin assignments and LED configurations:
 * - StatusLight: A single NeoPixel used for status indication (pin 10, GRB format).
 * - Center_NeoPixel_Chain: Three NeoPixels in an RGBW format for the center chain (pin 4).
 * - Left_NeoPixel_Chain: Two NeoPixels in an RGB format for the left chain (pin 6).
 * - Right_NeoPixel_Chain: Three NeoPixels in an RGB format for the right chain (pin 5).
 */
Adafruit_NeoPixel StatusLight(1, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Center_NeoPixel_Chain(3, 4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel Left_NeoPixel_Chain(2, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Right_NeoPixel_Chain(3, 5, NEO_GRB + NEO_KHZ800);

// ======== Constants ========

/*
 * A simple adjacency map representing connections between NeoPixels.
 * This structure can be used for animations or propagation effects.
 * Each index represents a pixel, with its adjacent pixels listed in the vector.
 */
vector<int> graph[8] = {
  {1},      // Pixel 0 is connected to Pixel 1
  {0, 2},   // Pixel 1 is connected to Pixels 0 and 2
  {1, 3},   // Pixel 2 is connected to Pixels 1 and 3
  {2, 4},   // Pixel 3 is connected to Pixels 2 and 4
  {3, 5},   // Pixel 4 is connected to Pixels 3 and 5
  {4, 6},   // Pixel 5 is connected to Pixels 4 and 6
  {5, 7},   // Pixel 6 is connected to Pixels 5 and 7
  {6, 0}    // Pixel 7 is connected to Pixels 6 and 0 (looping structure)
};

// ======== Button & Pattern Behavior Definitions ========
/*
 * Variables for handling button presses and pattern selection.
 * - patternIndex: Tracks the currently active pattern.
 * - buttonPressed: Tracks whether the button is currently pressed.
 * - buttonPressTime: Stores the timestamp of the last button press.
 * - longPressDuration: Defines the threshold (in ms) for detecting long presses.
 * - buttonInterrupt: Flags when a button press is detected.
 * - lampSleep: Flags when the device should enter sleep mode.
 * - buttonPressCount: Tracks the number of short presses.
 * - pressStartTime: Stores when a button press starts (for press duration calculation).
 * - buttonHeld: Tracks whether the button is currently being held.
 * - lastPressTime: Stores the timestamp of the last processed button press (debounce logic).
 */
int patternIndex = 1;
bool buttonPressed = false;
unsigned long buttonPressTime = 0;
const int longPressDuration = 2000; // 2 seconds for long press
volatile bool buttonInterrupt = false;
volatile bool lampSleep = false;
volatile int buttonPressCount = 0;
unsigned long pressStartTime = 0;
bool buttonHeld = false;
volatile unsigned long lastPressTime = 0;

// ======== LED Strand PWM Configuration ========
/*
 * Defines the PWM (Pulse Width Modulation) configuration for the LED strand.
 * - LEDC_CHANNEL: The PWM channel used for controlling brightness.
 * - LEDC_TIMER_BIT: The resolution of the PWM signal (8-bit = 0-255 levels).
 * - LEDC_BASE_FREQ: The frequency of the PWM signal (5 kHz for LED dimming).
 * - PWM_LED_PIN: The GPIO pin assigned for the LED strand control.
 */
const int LEDC_CHANNEL = 0;
const int LEDC_TIMER_BIT = 8;
const int LEDC_BASE_FREQ = 5000;
const int PWM_LED_PIN = 7;

// ======== Color Options ========
/*
 * Predefined color options for the NeoPixels, represented as RGB values.
 * The array stores different colors that can be randomly selected for lighting patterns.
 */
const int colorOptions[][3] = {
    {255, 0, 0},    // Red
    {0, 255, 0},    // Green
    {0, 0, 255},    // Blue
    {255, 255, 0},  // Yellow
    {0, 255, 255},  // Cyan
    {255, 0, 255},  // Magenta
    {255, 165, 0},  // Orange
    {75, 0, 130}    // Indigo
};

/*
 * Computes the number of available color options dynamically.
 * This ensures the size of colorOptions is correctly calculated for use in random selection.
 */
const int numColors = sizeof(colorOptions) / sizeof(colorOptions[0]);
