#include "Particle.h"
#include "Particle_Adafruit_IS31FL3731.h"

void setup();
void loop();

// If you're using the full breakout...
// Particle_Adafruit_IS31FL3731 ledmatrix = Particle_Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Particle_Adafruit_IS31FL3731_Wing ledmatrix = Particle_Adafruit_IS31FL3731_Wing();

// list of text to drawn on each frame
String frames[] = { "07", "16", "25", "34", "43", "52", "61", "70" };

void setup(){
  Serial.begin(9600);
  Serial.println("ISSI manual animation test");

  // call .begin(address) to initialize the driver.
  // .begin() will return false if the matrix was not found,
  // true if initialization succeeded
  if (!ledmatrix.begin()){
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 Found!");
  
  ledmatrix.setTextWrap(false);
  ledmatrix.setTextColor(32, 0);
  ledmatrix.setTextSize(1);
  
  // draw each frame
  for (size_t i = 0; i < arraySize(frames); ++i){
    ledmatrix.setFrame(i);
    ledmatrix.setCursor(0, 0);
    ledmatrix.fillScreen(0);
    ledmatrix.print(frames[i]);
    Serial.printlnf("Frame: %i", i);
  }
}

void loop(){
  int count = arraySize(frames);
  int ms = 250;

  // display each frame for 250 milliseconds
  for (int i = 0; i < count; ++i){
    ledmatrix.displayFrame(i);
    Serial.printlnf("Showing Frame: %i", i);
    delay(ms);
  }
  // now in reverse order
  for (int i = count - 2; i > 0; --i){
    ledmatrix.displayFrame(i);
    Serial.printlnf("Showing Frame: %i", i);
    delay(ms);
  }
}
