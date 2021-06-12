/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h> // To store on EEPROM Memory

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

bool WORLD[32][16]; // Creation of the wordl
int step_GOL; //used to know the generation

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(display.width()/2, display.height()/2, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(100);
  display.clearDisplay();

  display.drawRect(display.width()/2, display.height()/2, 4, 4, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...


    //Randomly initialazing the world for the first step
  randomSeed(analogRead(5));
  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 32; j++) {
      WORLD[i][j] = random(0, 2);
    }
  }
  gameOfLife();
  
}

void gameOfLife() {
    for(;;) {

  print_WORLD();

      if (step_GOL == 60) { // This if reboot the world after 60 generation to avoid static world
    step_GOL = 0;
    delay(500);
    randomSeed(analogRead(5));
    for (byte i = 0; i < 16; i++) {
      for (byte j = 0; j < 32; j++) {
        WORLD[i][j] = random(0, 2);
      }
    }

      //This double "for" is used to update the world to the next generation
  //The buffer state is written on the EEPROM Memory

  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 32; j++) {

      if (i == 0 || i == 15 || j == 0 || j == 31) // I choose to keep the border at 0
      {
        EEPROM.write(i * 31 + j , 0);
      }
      else {
        byte num_alive = WORLD[i - 1][j - 1] + WORLD[i - 1][j] + WORLD[i - 1][j + 1] + WORLD[i][j - 1] + WORLD[i][j + 1] + WORLD[i + 1][j - 1] + WORLD[i + 1][j] + WORLD[i + 1][j + 1];
        bool state = WORLD[i][j];

        //RULE#1 if you are surrounded by 3 cells --> you live
        if (num_alive == 3) {
          EEPROM.write(i * 31 + j , 1);
        }
        //RULE#2 if you are surrounded by 2 cells --> you stay in your state
        else if (num_alive == 2) {
          EEPROM.write(i * 31 + j , state);
        }
        //RULE#3 otherwise you die from overpopulation or subpopulation
        else {
          EEPROM.write(i * 31 + j , 0);
        }
      }
    }
  }

  //Updating the World
  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 32; j++) {
      WORLD[i][j] = EEPROM.read(i * 31 + j);
    }
  }

  //Displaying the world
  print_WORLD();
  display.clearDisplay();

  //Increasing the generation
  step_GOL++;


  
  }}
  }


// PRINT THE WORLD
void print_WORLD()
{
  int x,y;
  for (byte j = 0; j < 32; j++) {
    for (byte i = 0; i < 16; i++) {
      x = 4*i;
      y = 2*j;
      if (WORLD[i][j] == 0) {
        display.fillRect(y, x, 2, 2, SSD1306_WHITE);
      }
    }
  }
  display.display();
}
