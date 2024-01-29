#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
// Be sure to install Adafruit seesaw library!
#include <Adafruit_seesaw.h>
#include <Adafruit_TFTShield18.h>

Adafruit_TFTShield18 ss;


// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.
#define TFT_CS  10  // Chip select line for TFT display on Shield
#define TFT_DC   8  // Data/command line for TFT on Shield
#define TFT_RST  -1  // Reset line for TFT is handled by seesaw!

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Starting Position
int x = 16;
int y = 73;
bool mazeRed = false;
uint16_t customColor = tft.color565(0, 0, 75);

// Add a global variable to keep track of the score
int score = 0;

// Define the goal coordinates
const int GOAL_X = 114;
const int GOAL_Y = 129;

// Wall coordinates
const int WALL_COORDS[][4] = {
  //  x1   y1   x2   y2
  //  0    0    0    0
  {0, 0, 127, 0},     // top
  {127, 0, 127, 159}, // right
  {0, 159, 127, 159}, // bottom
  {0, 0, 0, 159},     // left
  // Draw the walls of the maze
  //Horizontal
  {9, 24, 121, 24},
  {23, 38, 65, 38},
  {65, 52, 107, 52},
  {1, 66, 37, 66},
  {1, 80, 8, 80},
  {51, 66, 93, 66},
  {23, 80, 65, 80},
  {93, 80, 107, 80},
  {9, 108, 37, 108},
  {9, 108, 37, 108},
  {37, 94, 51, 94},
  {65, 108, 79, 108},
  {93, 108, 107, 108},
  {23, 122, 65, 122},
  {79, 122, 93, 122},
  {9, 136, 121, 136},
  //Vertical
  {9, 24, 9, 66},
  {93, 24, 93, 38},
  {121, 24, 121, 122},
  {23, 38, 23, 52},
  {51, 38, 51, 66},
  {79, 38, 79, 52},
  {107, 38, 107, 94},
  {37, 52, 37, 66},
  {23, 66, 23, 94},
  {79, 66, 79, 122},
  {9, 80, 9, 136},
  {65, 80, 65, 108},
  {93, 80, 93, 108},
  {37, 94, 37, 108},
  {51, 94, 51, 122},
  {107, 108, 107, 136}
};


void setup() {

  Serial.begin(9600);

  // disable TFT
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);

  // Start seesaw helper chip
  if (!ss.begin()){
    Serial.println("seesaw could not be initialized!");
    while(1);
  }

  // Start set the backlight off
  ss.setBacklight(TFTSHIELD_BACKLIGHT_OFF);
  // Reset the TFT
  ss.tftReset();

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.fillScreen(customColor);

  for (int32_t i=TFTSHIELD_BACKLIGHT_OFF; i<TFTSHIELD_BACKLIGHT_ON; i+=100) {
    ss.setBacklight(i);
    delay(3);
  }

  // Draw Maze
  mazeWalls(ST77XX_MAGENTA);

  // Draw the player at the starting position
  drawPlayer(ST77XX_WHITE);

  // Draw the goal as a colored square or some distinct shape
  drawGoal(ST77XX_YELLOW);
  
}

void drawPlayer(uint16_t color){
  tft.drawPixel(x, y  , color);
  tft.drawPixel(x, y-1, color);
  tft.drawPixel(x-1, y, color);
  tft.drawPixel(x, y+1, color);
  tft.drawPixel(x+1, y, color);
}

void loop() {

  uint32_t buttons = ss.readButtons();

  if (mazeRed){
      mazeWalls(ST77XX_MAGENTA);
      mazeRed = false; // Reset the flag after drawing walls in magenta
  }

  // Check if the user's next move will intersect with a wall
  bool canMove = true;
  bool buttonPressed = false;
  int nextX = x, nextY = y;

  if(! (buttons & TFTSHIELD_BUTTON_LEFT)) {
    buttonPressed = true;
    nextY -= 1;
    if (nextX < 0 || nextX > 127) { // out of bounds
      canMove = false;
    } else {
      // Check if the next move intersects with a wall
      if (wallCollision(nextX, nextY)) canMove = false;
    }
  } else if(! (buttons & TFTSHIELD_BUTTON_UP)) {
    buttonPressed = true;
    nextX += 1;
    if (nextY < 0 || nextY > 159) { // out of bounds
      canMove = false;
    } else {
      // Check if the next move intersects with a wall
      if (wallCollision(nextX, nextY)) canMove = false;
    }
  } else if(! (buttons & TFTSHIELD_BUTTON_RIGHT)) {
    buttonPressed = true;
    nextY += 1;
    if (nextX < 0 || nextX > 127) { // out of bounds
      canMove = false;
    } else {
      // Check if the next move intersects with a wall
      if (wallCollision(nextX, nextY)) canMove = false;
    }
  } else if(! (buttons & TFTSHIELD_BUTTON_DOWN)) {
    buttonPressed = true;
    nextX -= 1;
    if (nextY < 0 || nextY > 159) { // out of bounds
      canMove = false;
    } else {
      // Check if the next move intersects with a wall
      if (wallCollision(nextX, nextY)) canMove = false;
    }
  } else if(! (buttons & TFTSHIELD_BUTTON_1)) {
    // line draw
    testlines(ST77XX_YELLOW);
    delay(500);
    // optimized lines
    testfastlines(ST77XX_RED, ST77XX_BLUE);
    delay(500);
    redrawMazeAndPlayer();
  } else if(! (buttons & TFTSHIELD_BUTTON_2)) {
    testdrawrects(ST77XX_GREEN);
    delay(500);
    testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
    delay(500);
    redrawMazeAndPlayer();
  } else if(! (buttons & TFTSHIELD_BUTTON_3)) {
    tft.fillScreen(ST77XX_BLACK);
    testfillcircles(10, ST77XX_BLUE);
    testdrawcircles(10, ST77XX_WHITE);
    delay(500);
    testroundrects();
    delay(500);
    redrawMazeAndPlayer();
  } else if(! (buttons & TFTSHIELD_BUTTON_IN)) {
    testtriangles();
    delay(500);
    redrawMazeAndPlayer();
  }

  // Move the user if allowed
  if(buttonPressed){
    if (canMove) {
      //erase last pos..
      drawPlayer(ST77XX_BLACK);
      x = nextX;
      y = nextY;
      drawPlayer(ST77XX_WHITE);
      mazeWalls(ST77XX_MAGENTA);
      delay(5); // controls speed of player motion
    }
  }

  // After moving the player, check if the new position is at the goal
  if (x >= GOAL_X-6 && y >= GOAL_Y-6) {
    // Increment the score
    score++;

    // Display a message about the reward
    tft.fillScreen(ST77XX_BLACK); // Clear the screen
    tft.setRotation(1); // Rotate text 90 degrees clockwise
    tft.setCursor(0, 0); // Set cursor to top left corner
    tft.setTextColor(ST77XX_WHITE); // Set text color
    tft.setTextSize(3); // Set text size
    tft.print("Reward! Score: ");
    tft.print(score); // Show the current score
    tft.setRotation(0); // Reset the rotation for other text

    // Delay a bit to allow the player to see the message
    delay(2000);

    // Reset player position to start
    x = 16; // Starting X coordinate
    y = 73; // Starting Y coordinate

    // Redraw the maze and the player
    redrawMazeAndPlayer();
  }
}

// Define the function to redraw the maze, player, and goal
void redrawMazeAndPlayer() {
  tft.fillScreen(customColor);  // Fill the screen with the background color
  mazeWalls(ST77XX_MAGENTA);    // Draw the maze walls
  drawPlayer(ST77XX_WHITE);      // Draw the player
  drawGoal(ST77XX_YELLOW);          // Draw the goal
}

bool wallCollision(int nx, int ny) {
  bool result = false;
  // Check if the next move intersects with a wall
  for (int i = 0; i < sizeof(WALL_COORDS) / sizeof(WALL_COORDS[0]); i++) {
    int x1 = WALL_COORDS[i][0];
    int y1 = WALL_COORDS[i][1];
    int x2 = WALL_COORDS[i][2];
    int y2 = WALL_COORDS[i][3];
    if (x1 == x2 && x1 == nx && y1 <= ny && y2 >= ny ||
        y1 == y2 && y1 == ny && x1 <= nx && x2 >= nx) {
      result = true;
      //flash maze walls red..
      mazeWalls(ST77XX_RED);
      mazeRed = true;
      break;
    }
  }
  return result;
}

void mazeWalls(uint16_t color) {
  // Draw the walls of the maze
  for (int i = 0; i < sizeof(WALL_COORDS) / sizeof(WALL_COORDS[0]); i++) {
    int x1 = WALL_COORDS[i][0];
    int y1 = WALL_COORDS[i][1];
    int x2 = WALL_COORDS[i][2];
    int y2 = WALL_COORDS[i][3];
    tft.drawLine(x1, y1, x2, y2, color);
  }
}

void drawGoal(uint16_t color){
  // Draw the goal as a small square or dot at the GOAL_X and GOAL_Y coordinates
  tft.fillRect(GOAL_X-5, GOAL_Y-5, 11, 11, color); // Drawing a 5x5 pixel square as the goal
}

void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(20);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(20);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(20);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(20);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(20);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(20);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(20);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(20);
  }
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
    delay(20);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
    delay(20);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
    delay(20);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
    delay(20);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
    delay(20);
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
    delay(20);
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
    delay(20);
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
      delay(5);
    }
    color+=100;
    delay(20);
  }
}
