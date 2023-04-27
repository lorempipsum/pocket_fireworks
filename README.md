# Pocket Fireworks
Pocket Fireworks is a set of animations for monochrome OLED displays based on SSD1306 drivers. The animations are designed for Arduino boards and are displayed on 128x32 or 128x64 pixel OLED displays connected via I2C.

## Animations
Gif Displayer (gif_displayer.ino) - Displays a small bitmap image.
Gravity Worm (gravity_worm.ino) - Simulates "snowflakes" affected by gravity, creating a worm-like animation.
Rotating Cube (rotating_cube.ino) - Displays a 3D cube rotating around its center.

## Requirements
Arduino board
Monochrome OLED display (128x32 or 128x64 pixels) based on SSD1306 drivers
Arduino IDE
### Libraries
Install the following libraries in your Arduino IDE:

Adafruit_GFX
Adafruit_SSD1306
Wire
SPI
EEPROM (only for gravity_worm.ino)

### Setup and Usage
Clone the repository.
Open the desired animation file (.ino) in the Arduino IDE.
Connect the OLED display to the Arduino board using I2C.
Upload the sketch to your Arduino board.
The selected animation will run on the OLED display.
License
This project is released under the BSD license. Please refer to the license.txt file for more information.
