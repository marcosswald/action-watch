#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SHARP_SCK  12
#define SHARP_MOSI 11
#define SHARP_SS   10

// Set the size of the display here, e.g. 144x168!
Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);
// The currently-available SHARP Memory Display (144x168 pixels)
// requires > 4K of microcontroller RAM

#define BLACK 0
#define WHITE 1

int minorHalfSize; // 1/2 of lesser of display width or height

void setup() {
  Serial.println("GPS echo test");
  Serial.begin(9600);
  Serial1.begin(9600);

  // start & clear the display
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("ActionWatch 1.0");
  display.refresh();

  // led
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);
  }
}
