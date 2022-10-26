#include "Particle.h"
#include "Particle_Adafruit_IS31FL3731.h"

void setup();
void loop();

// If you're using the full breakout...
// Particle_Adafruit_IS31FL3731 ledmatrix = Particle_Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Particle_Adafruit_IS31FL3731_Wing ledmatrix = Particle_Adafruit_IS31FL3731_Wing();


// The lookup table to make the brightness changes be more visible
uint8_t sweep[] = { 1, 2, 3, 4, 6, 8, 10, 15, 20, 30, 40, 60, 60, 40, 30, 20, 15, 10, 8, 6, 4, 3, 2, 1 };

void setup(){
  Serial.begin(9600);
  Serial.println("ISSI swirl test");

  // call .begin(address) to initialize the driver.
  // .begin() will return false if the matrix was not found, and true if initialization succeeded
  if (!ledmatrix.begin()){
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");
}

void loop(){
  // animate over all the pixels, and set the brightness from the sweep table
  for (uint8_t incr = 0; incr < 24; incr++){
    for (uint8_t x = 0; x < 16; x++){
      for (uint8_t y = 0; y < 9; y++){
        ledmatrix.drawPixel(x, y, sweep[(x + y + incr) % 24]);
      }
    }
  }
  delay(20);
}