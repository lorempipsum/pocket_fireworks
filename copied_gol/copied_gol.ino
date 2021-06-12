/*****************************************************************
 *****************************************************************

  CONWAY'S GAME OF LIFE V1

  DATE : 19 JAN 2018

  CONCEPTOR : AERODYNAMICS

  This program intends to run a CONWAYS'S Game of Life on an Arduino
  and to display it on a 16x32 LED MATRIX


* *****************************************************************
*******************************************************************/

//////////////////////////
//      LIBRARIES       //
//////////////////////////


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

//////////////////////////
//       VARIABLES      //
//////////////////////////

//Definition of the pattern if you use the pattern initialization
int pattern_size[] = {7, 22}; // row x Column
char pattern_init[] =
  ".........*,\
.......*.*,\
......*.*,\
**...*..*...........**,\
**....*.*...........**,\
.......*.*,\
.........*!";

int GOL_HEIGHT = 16;
int GOL_WIDTH = 32;

bool WORLD[16][32]; // Creation of the wordl
int step_GOL; //used to know the generation

//////////////////////////
//       OBJECTS        //
//////////////////////////



/*************************************************************************************************************/
//////////////////////////
//          SETUP       //
//////////////////////////
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
  
  //Randomly initialazing the world for the first step
  randomSeed(analogRead(5));
  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 32; j++) {
      WORLD[i][j] = random(0, 2);
    }
  }

  //init_WORLD(); // Uncomment if you want to init with a specific pattern

  step_GOL = 0;
  print_WORLD(); //Display the first generation

}


/*************************************************************************************************************/
//////////////////////////
//    LOOP FUNCTION     //
//////////////////////////
void loop() {
  if (step_GOL == 60) { // This if reboot the world after 60 generation to avoid static world
    step_GOL = 0;
    randomSeed(analogRead(5));
    for (byte i = 0; i < 16; i++) {
      for (byte j = 0; j < 32; j++) {
        WORLD[i][j] = random(0, 2);
      }
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
  display.display();
  display.clearDisplay();

  //Increasing the generation
  step_GOL++;

}

/*************************************************************************************************************/
//////////////////////////
//       FUNCTIONS      //
//////////////////////////


// PRINT THE WORLD
void print_WORLD()
{
  int x,y;
  for (byte j = 0; j < 32; j++) {
    for (byte i = 0; i < 16; i++) {
      if (WORLD[i][j] > 0) {
        x = 4*j;
        y = 2*i;
        // display.drawPixel(x, y, SSD1306_WHITE);
        display.fillRect(x, y, 4, 2, SSD1306_WHITE);
      }
    }
  }
}

//Those two function are used to display the world on the serial monitor
//Not beautiful but useful to debug

void print_WORLD_SERIAL()
{
  clearscreen();
  Serial.print("Step = "); Serial.println(step_GOL);
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 32; j++) {
      if (WORLD[i][j] == 0) {
        Serial.print(".");
        Serial.print(" ");
      }
      else
      {
        Serial.print("*");
        Serial.print(" ");
      }
    }
    Serial.println("");
  }
  Serial.println("");

}

void clearscreen() {
  for (int i = 0; i < 10; i++) {
    Serial.println("\n\n\n\n");
  }
}

//This function is used to init the world with a know pattern
//It read . and * to convert them to 0 and 1.
//Inspired from life 1.05 format
// NB : this function needs improvment to center the pattern

void init_WORLD() {
  int k = 0;
  int row = 0;
  int column = 0;
  while (pattern_init[k] != '!') {
    if (pattern_init[k] == ',') {
      row++;
      k++;
      column = 0;
    }
    else if (pattern_init[k] == '.') {
      WORLD[row + 2][column + 4] = 0;
      k++;
      column ++;
    }
    else  {
      WORLD[row + 2][column + 4] = 1;
      k++;
      column ++;
    }
  }
}
