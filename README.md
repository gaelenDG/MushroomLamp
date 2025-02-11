
# Interactive LED Lamp with touch control

## Overview

This project is an ESP32-controlled interactive lamp featuring NeoPixel LED chains and a capacitive touch sensor (TTP223B) as the primary input. The lamp supports multiple lighting patterns, a breathing effect, and a long-press sleep mode.

I embedded a series of AdaFruit NeoPixels in a series of "mushrooms" consisting of shaped silicone caulk. The mushrooms were installed on a piece of bark (found on a log in my backyard) and made to look as natural as possible.

The whole project was donated to a local middle school classroom for interactive learning about hobby electronics. 

## Features

* Multiple LED Patterns: The lamp includes color transitions, independent breathing effects for each LED, and other dynamic lighting effects.
* Capacitive Touch Control: Users can cycle through lighting modes with a short touch and put the lamp to sleep with a long touch (2 seconds).
* Power Efficiency: The ESP32 enters low-power sleep mode when inactive, saving energy.
* Modular Code Structure: The scripts are organized for readability and maintainability, with separate files for pattern functions, initialization, and helper utilities.

## Hardware Requirements

* Microcontroller: ESP32-C3
* LEDs: NeoPixel (WS2812B) chains
* Touch Sensor: TTP223B capacitive touch module
* Power Supply: 5V USB or battery (USB-C input to the ESP32-C3 onboard power)

## Software Requirements

* PlatformIO (or Arduino IDE)
* FastLED or Adafruit NeoPixel Library for LED control
* ESP32 framework (for handling GPIO and sleep modes)

## Setup & Usage

1) Wiring
	* Connect the NeoPixels to GPIO 5.
	* Connect the TTP223B sensor to GPIO 0 (or another digital/analog pin as needed).
	* Use a pull-up resistor on the touch sensor if needed to ensure correct detection.

2) Installation
	* Clone the repository:

```
git clone https://github.com/gaelenDG/MushroomLamp
cd MushroomLamp
```

## Code structure

```
led-lamp/
│── src/
│   ├── main.cpp        # Handles setup and main loop
│   ├── patterns.cpp    # LED pattern functions
│   ├── touch_input.cpp # Touch sensor interrupt handling
│   ├── config.h        # Pin definitions and settings
│── platformio.ini      # PlatformIO project configuration
│── README.md           # Project documentation
```

## Demo photos and video


![Wiring neopixel chains on the left and right sides of the base.](/DemoMedia/IMG_4222.png)

![Testing NeoPixel chains on the left and right side of the base.](/DemoMedia/IMG_4220.png)


![Testing light patterns with center mushroom wired in place.](/DemoMedia/IMG_4231.mp4)

![Testing button debounce.](/DemoMedia/IMG_4244.mp4)


## Future improvements

* Higher quality button (or more optimized installation) for reduced hysteresis.
* Customizable brightness levels via additional touch gestures.
* Integration with Bluetooth or Wi-Fi for remote control.
* More complex animation patterns.
* Built in microphone for responsiveness to ambient sound.
