#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS    10
#define TFT_DC    8
#define TFT_RST   9
#define TFT_SCLK  12
#define TFT_MOSI  11

SPIClass mySPI(HSPI);
Adafruit_ST7735 tft = Adafruit_ST7735(&mySPI, TFT_CS, TFT_DC, TFT_RST);

#define BTN_UP     2
#define BTN_DOWN   3
#define BTN_LEFT   4
#define BTN_RIGHT  5
#define BTN_START  7

// Larry sprite
static const uint32_t larry_data[1][100] = {{
  0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xff000000,
  0xff000000, 0xffffff00, 0xff000000, 0xffffff00, 0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xffffff00, 0xff000000,
  0xff000000, 0xffffffff, 0xffffff00, 0xffffff00, 0xff000000, 0xff000000, 0xffffffff, 0xffffff00, 0xffffff00, 0xff000000,
  0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xff000000, 0xffffff00, 0xffffff00, 0xff000000, 0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00
}};

// Box sprite
static const uint32_t boxSprite[1][100] = {{
  0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513,
  0xff8b4513, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff8b4513,
  0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513
}};

const int size = 10;
const int step = 5;
const int WALL = 10;
const int wallColumnX = 15;

const int START_PLAYER_X = 30;
const int START_PLAYER_Y = 30;

const int START_BOX_X = 60;
const int START_BOX_Y = 30;

const int START_BOX2_X = 60;
const int START_BOX2_Y = 60;

const int GOAL_X = 100;
const int GOAL_Y = 30;

const int GOAL2_X = 100;
const int GOAL2_Y = 60;

int playerX = START_PLAYER_X, playerY = START_PLAYER_Y;
int boxX = START_BOX_X, boxY = START_BOX_Y;
int box2X = START_BOX2_X, box2Y = START_BOX2_Y;

bool victory = false;

void setup() {
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);
  drawScene();
}

void loop() {
  if (victory) return;
  int dx = 0, dy = 0;
  if (digitalRead(BTN_UP) == LOW) dy = -step;
  if (digitalRead(BTN_DOWN) == LOW) dy = step;
  if (digitalRead(BTN_LEFT) == LOW) dx = -step;
  if (digitalRead(BTN_RIGHT) == LOW) dx = step;

  if (digitalRead(BTN_START) == LOW) {
    clearSprite(playerX, playerY);
    clearSprite(boxX, boxY);
    clearSprite(box2X, box2Y);
    tft.fillScreen(ST77XX_BLACK);
    playerX = START_PLAYER_X;
    playerY = START_PLAYER_Y;
    boxX = START_BOX_X;
    boxY = START_BOX_Y;
    box2X = START_BOX2_X;
    box2Y = START_BOX2_Y;
    victory = false;
    drawScene();
    delay(300);
    return;
  }

  if (dx != 0 || dy != 0) {
    int newPlayerX = playerX + dx;
    int newPlayerY = playerY + dy;

    if (!isInside(newPlayerX, newPlayerY) || isWall(newPlayerX, newPlayerY)) return;

    bool empujaCaja1 = isColliding(newPlayerX, newPlayerY, boxX, boxY);
    bool empujaCaja2 = isColliding(newPlayerX, newPlayerY, box2X, box2Y);

    if (empujaCaja1 || empujaCaja2) {
      int& bX = empujaCaja1 ? boxX : box2X;
      int& bY = empujaCaja1 ? boxY : box2Y;
      int newBoxX = bX + dx;
      int newBoxY = bY + dy;
      if (isInside(newBoxX, newBoxY) && !isWall(newBoxX, newBoxY) &&
          !isColliding(newBoxX, newBoxY, empujaCaja1 ? box2X : boxX, empujaCaja1 ? box2Y : boxY)) {
        clearSprite(bX, bY);
        bX = newBoxX;
        bY = newBoxY;
        clearSprite(playerX, playerY);
        playerX = newPlayerX;
        playerY = newPlayerY;
        drawScene();
      }
    } else {
      clearSprite(playerX, playerY);
      playerX = newPlayerX;
      playerY = newPlayerY;
      drawScene();
    }

    if (((boxX == GOAL_X && boxY == GOAL_Y) && (box2X == GOAL2_X && box2Y == GOAL2_Y)) ||
        ((boxX == GOAL2_X && boxY == GOAL2_Y) && (box2X == GOAL_X && box2Y == GOAL_Y))) {
      tft.setTextColor(ST77XX_WHITE);
      tft.setTextSize(2);
      tft.setCursor(20, 80);
      tft.println("VICTORIA!");
      victory = true;
    }

    delay(100);
  }
}

void drawScene() {
  tft.fillRect(GOAL_X, GOAL_Y, size, size, ST77XX_GREEN);
  tft.fillRect(GOAL2_X, GOAL2_Y, size, size, ST77XX_GREEN);

  tft.fillRect(0, 0, tft.width(), WALL, ST77XX_WHITE);
  tft.fillRect(0, tft.height() - WALL, tft.width(), WALL, ST77XX_WHITE);
  tft.fillRect(0, 0, WALL, tft.height(), ST77XX_WHITE);
  tft.fillRect(tft.width() - WALL, 0, WALL, tft.height(), ST77XX_WHITE);

  for (int y = WALL; y < tft.height() - WALL; y += size) {
    tft.fillRect(wallColumnX, y, size, size, ST77XX_WHITE);
  }

  drawLarry(playerX, playerY);
  drawBox(boxX, boxY);
  drawBox(box2X, box2Y);
}

void drawLarry(int x, int y) {
  for (int r = 0; r < 10; r++) {
    for (int c = 0; c < 10; c++) {
      uint32_t color32 = larry_data[0][r * 10 + c];
      if ((color32 & 0xFFFFFF) != 0x000000) {
        uint16_t color16 = tft.color565((color32 >> 16) & 0xFF, (color32 >> 8) & 0xFF, color32 & 0xFF);
        tft.drawPixel(x + c, y + r, color16);
      }
    }
  }
}

void drawBox(int x, int y) {
  for (int r = 0; r < 10; r++) {
    for (int c = 0; c < 10; c++) {
      uint32_t color32 = boxSprite[0][r * 10 + c];
      if ((color32 & 0xFFFFFF) != 0x000000) {
        uint16_t color16 = tft.color565((color32 >> 16) & 0xFF, (color32 >> 8) & 0xFF, color32 & 0xFF);
        tft.drawPixel(x + c, y + r, color16);
      }
    }
  }
}

void clearSprite(int x, int y) {
  tft.fillRect(x, y, size, size, ST77XX_BLACK);
}

bool isInside(int x, int y) {
  return x >= WALL && x <= tft.width() - WALL - size && y >= WALL && y <= tft.height() - WALL - size;
}

bool isColliding(int ax, int ay, int bx, int by) {
  int margin = 4;
  return abs(ax - bx) < size - margin && abs(ay - by) < size - margin;
}

bool isWall(int x, int y) {
  return x >= wallColumnX && x < wallColumnX + size;
}