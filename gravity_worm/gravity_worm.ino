

//////////////////////////
//      LIBRARIES       //
//////////////////////////
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h> // To store on EEPROM Memory

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMBER_OF_FLAKES 4
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define DIRECTION 3

float GRAVITY = 1;

int8_t flake, flakes[NUMBER_OF_FLAKES][4];

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(display.width() / 2, display.height() / 2, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(50);
  display.clearDisplay();

  display.fillRect(display.width() / 2, display.height() / 2, 4, 4, SSD1306_WHITE);
  display.display();
  delay(100);


  // Initialize 'snowflake' positions
  for (flake = 0; flake < NUMBER_OF_FLAKES; flake++) {
    flakes[flake][XPOS]   = random(1 , display.width());
    flakes[flake][YPOS]   = 64;
    flakes[flake][DIRECTION] = 1;
    flakes[flake][DELTAY] = 6;
    Serial.print(F("x: "));
    Serial.print(flakes[flake][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(flakes[flake][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(flakes[flake][DELTAY], DEC);
    Serial.print(F(" dir: "));
    Serial.println(flakes[flake][DIRECTION], DEC);
  }
}


void loop() {

  display.clearDisplay();

  // Draw each flake
  for (flake = 0; flake < NUMBER_OF_FLAKES; flake++) {
    display.fillCircle(flakes[flake][XPOS], flakes[flake][YPOS], 4, SSD1306_WHITE);
  }
  display.display();

  // Update coordinates
  for (flake = 0; flake < NUMBER_OF_FLAKES; flake++) {
    flakes[flake][DELTAY] -= (GRAVITY*flakes[flake][DIRECTION]);
    
    if (flakes[flake][DELTAY] <= 2) {
      flakes[flake][DIRECTION] = 1;
    }
    flakes[flake][YPOS] -=  flakes[flake][DIRECTION]*(flakes[flake][DELTAY]);

    // if flakes is off bottom
    if (flakes[flake][YPOS] > (random(0, 20) + display.height())) {
      flakes[flake][YPOS] = 64;
      flakes[flake][XPOS] = random(0, display.width());
      flakes[flake][DELTAY] = random(6, 12);
      flakes[flake][DIRECTION] = 1;
    }
  }


}
