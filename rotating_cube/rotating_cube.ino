#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// interval between the animation frames
int frame_delay = 70;

// Cube side length
int a = 16;
// length from centre of cube to rotation side
int r = 20;
int x0 = display.width() / 2;
int y0 = display.height() / 2 - a/2;
int yCenter = y0;
int x, y;
float alpha = 0;
float bobAlpha = 0;

int BOB_AMOUNT = 4;


void setup() {
  Serial.begin(9600);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  // display.display();
  // delay(2000); // Pause for 2 seconds
}

void loop() {

  // Diplay Animation

  // Frame1
  display.clearDisplay();
  drawCube();
  display.display();

}

void drawCube() {
  display.clearDisplay();

  alpha = alpha + 0.1`;
  bobAlpha = bobAlpha + 0.2;
  
  y0 = yCenter + BOB_AMOUNT*(cos(bobAlpha));

  drawCorner(0, alpha);
  drawCorner(1.57, alpha);
  drawCorner(1.57 * 2, alpha);
  drawCorner(1.57 * 3, alpha);
  display.display();
}

void drawCorner(float phase, float alpha) {
  int x1,y1,x2,y2;

  x = floor(x0 + r * sin(alpha + phase));
  y = floor(y0 - r * cos(alpha + phase));
  x1 = floor(x0 + r * sin(alpha + phase + 1.57));
  y1 = floor(y0 - r * cos(alpha + phase + 1.57));
  x2 = floor(x0 + r * sin(alpha + phase - 1.57));
  y2 = floor(y0 - r * cos(alpha + phase - 1.57));
  display.drawPixel(x, y, SSD1306_WHITE);
  display.drawLine(x, y, x, y+a, SSD1306_WHITE);
  display.drawLine(x, y, x1, y1, SSD1306_WHITE);
  display.drawLine(x, y+a, x1, y1+a, SSD1306_WHITE);

}
