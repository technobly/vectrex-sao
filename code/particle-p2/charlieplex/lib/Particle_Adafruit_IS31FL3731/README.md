# Particle_Adafruit_IS31FL3731

A Particle library for the Adafruit IS31FL3731 [Charlieplex LED driver](https://learn.adafruit.com/i31fl3731-16x9-charliplexed-pwm-led-driver) and [CharliePlex FeatherWing](https://learn.adafruit.com/adafruit-15x7-7x15-charlieplex-led-matrix-charliewing-featherwing). Forked from [adafruit/Adafruit_IS31FL3731](https://github.com/adafruit/Adafruit_IS31FL3731)


## Installation

via the Particle CLI:

```
particle library add Particle_Adafruit_IS31FL3731
```

via Particle Workbench:

1. run the `Particle: Install Library` command
2. enter `Particle_Adafruit_IS31FL3731`


## Usage

Start by creating a new matrix object with something like:

```
#include "Particle_Adafruit_IS31FL3731.h"

// If you're using the full breakout...
// Particle_Adafruit_IS31FL3731 ledmatrix = Particle_Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Particle_Adafruit_IS31FL3731_Wing ledmatrix = Particle_Adafruit_IS31FL3731_Wing();
```

Then in your setup, call `.begin(<address>)` to initialize the driver. `.begin()` will return false if the matrix was not found, and `true`  if initialization succeeded.

```
void setup(){
  if (!ledmatrix.begin()){
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");
}
```

You can then draw to the display. Note that since we write directly to the driver RAM, any pixels 'drawn' will appear immediately.

You can start with `drawPixel(x, y, brightness)` where `x` ranges between `0` and `14` inclusive, and `y` ranges between `0` and `7` inclusive. Brightness is the PWM of the LED, `0` is off, and `255` is all the way on.

This loop will light up every LED in increasing brightness:

```
int i = 0;
for (uint8_t x = 0; x < 15; ++x) {
  for (uint8_t y = 0; y < 7; ++y) {
    ledmatrix.drawPixel(x, y, ++i]);
  }
}
```

See the [examples](examples) folder for more details.
