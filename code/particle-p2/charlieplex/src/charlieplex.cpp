#include "Particle.h"
#include "Particle_Adafruit_IS31FL3731.h"

SYSTEM_THREAD(ENABLED);

#define FRAMES_DEMO (0)

// If you're using the full breakout...
// Particle_Adafruit_IS31FL3731 ledmatrix = Particle_Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Particle_Adafruit_IS31FL3731_Wing ledmatrix = Particle_Adafruit_IS31FL3731_Wing();


#if FRAMES_DEMO
// list of text to drawn on each frame
String frames[] = { "20", "21", "22", "23", "24", "25", "26", "27"}; // 8 frames max!
#else
// The lookup table to make the brightness changes be more visible
uint8_t sweep[] = { 1, 2, 3, 4, 6, 8, 10, 15, 20, 30, 40, 60, 60, 40, 30, 20, 15, 10, 8, 6, 4, 3, 2, 1 };
#endif // FRAMES_DEMO

void setup() {
    Serial.begin(9600);
    // delay(5000);
#if FRAMES_DEMO
    Serial.println("ISSI frames demo");
#else
    Serial.println("ISSI swirl test");
#endif // FRAMES_DEMO

    // call .begin(address) to initialize the driver.
    // .begin() will return false if the matrix was not found, and true if initialization succeeded
    if (!ledmatrix.begin()) {
        Serial.println("IS31 not found");
        while (1);
    }
    Serial.println("IS31 found!");

#if FRAMES_DEMO
    ledmatrix.setTextWrap(false);
    ledmatrix.setTextColor(32, 0);
    ledmatrix.setTextSize(1);

    // draw each frame
    for (size_t i = 0; i < arraySize(frames); ++i) {
        ledmatrix.setFrame(i);
        ledmatrix.setCursor(0, 0);
        ledmatrix.fillScreen(0);
        ledmatrix.print(frames[i]);
        Serial.printlnf("Frame: %i", i);
    }
#endif // FRAMES_DEMO
}

void loop() {
#if FRAMES_DEMO
    int count = arraySize(frames);
    int ms = 250;

    // display each frame for 250 milliseconds
    for (int i = 0; i < count; ++i) {
        ledmatrix.displayFrame(i);
        Serial.printlnf("Showing Frame: %i", i);
        delay(ms);
    }
    // now in reverse order
    for (int i = count - 2; i > 0; --i) {
        ledmatrix.displayFrame(i);
        Serial.printlnf("Showing Frame: %i", i);
        delay(ms);
    }
#else
    // // animate over all the pixels, and set the brightness from the sweep table
    // for (uint8_t incr = 0; incr < 24; incr++) {
    //     for (uint8_t x = 0; x < 10; x++) {
    //         for (uint8_t y = 0; y < 7; y++) {
    //             ledmatrix.drawPixel(x, y, sweep[(x + y + incr) % 24]);
    //         }
    //     }
    // }
    // delay(20);

    static int8_t counter = 0;
    // BOTTOM
    static uint8_t brightness = 30;
    for (int8_t x = 0; x < 7; x++) {
        if ((x + counter) % 4 == 0) {
            ledmatrix.drawPixel(9, x, brightness);
        } else {
            ledmatrix.drawPixel(9, x, 0);
        }
    }
    // RIGHT
    for (int8_t y = 0; y < 10; y++) {
        if ((y - counter) % 4 == 0) {
            ledmatrix.drawPixel(y, 6, brightness);
        } else {
            ledmatrix.drawPixel(y, 6, 0);
        }
    }
    // TOP
    for (int8_t x = 0; x < 7; x++) {
        if ((x - counter) % 4 == 0) {
            ledmatrix.drawPixel(0, x, brightness);
        } else {
            ledmatrix.drawPixel(0, x, 0);
        }
    }
    // LEFT
    for (int8_t y = 0; y < 10; y++) {
        if ((y + counter) % 4 == 0) {
            ledmatrix.drawPixel(y, 0, brightness);
        } else {
            ledmatrix.drawPixel(y, 0, 0);
        }
    }
    counter++;
    if (counter > 3) {
        counter = 0;
    }
    delay(20);
#endif // FRAMES_DEMO
}
