#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// ---------- Pines pantalla ----------
#define TFT_CS    10
#define TFT_DC    8
#define TFT_RST   9
#define TFT_SCLK  12
#define TFT_MOSI  11

SPIClass mySPI(HSPI);
Adafruit_ST7735 tft = Adafruit_ST7735(&mySPI, TFT_CS, TFT_DC, TFT_RST);

// ---------- Pines botones ----------
#define BTN_UP     2
#define BTN_DOWN   3
#define BTN_LEFT   4
#define BTN_RIGHT  5
#define BTN_START  7

// ---------- Sprite de caracol (10x10) ----------
const uint16_t snailSprite[10][10] = {
  {0,0,0,ST77XX_YELLOW,ST77XX_YELLOW,0,0,0,0,0},
  {0,0,ST77XX_YELLOW,ST77XX_MAGENTA,ST77XX_MAGENTA,ST77XX_YELLOW,0,0,0,0},
  {0,ST77XX_YELLOW,ST77XX_MAGENTA,ST77XX_WHITE,ST77XX_WHITE,ST77XX_MAGENTA,ST77XX_YELLOW,0,0,0},
  {ST77XX_YELLOW,ST77XX_MAGENTA,ST77XX_WHITE,ST77XX_BLACK,ST77XX_BLACK,ST77XX_WHITE,ST77XX_MAGENTA,ST77XX_YELLOW,0,0},
  {ST77XX_YELLOW,ST77XX_MAGENTA,ST77XX_WHITE,ST77XX_BLACK,ST77XX_BLACK,ST77XX_WHITE,ST77XX_MAGENTA,ST77XX_YELLOW,0,0},
  {0,ST77XX_YELLOW,ST77XX_MAGENTA,ST77XX_WHITE,ST77XX_WHITE,ST77XX_MAGENTA,ST77XX_YELLOW,0,0,0},
  {0,0,ST77XX_YELLOW,ST77XX_MAGENTA,ST77XX_MAGENTA,ST77XX_YELLOW,0,0,0,0},
  {0,0,0,ST77XX_YELLOW,ST77XX_YELLOW,0,0,0,0,0},
  {0,0,0,ST77XX_WHITE,0,ST77XX_WHITE,0,0,0,0},
  {0,0,0,ST77XX_WHITE,0,ST77XX_WHITE,0,0,0,0}
};

// ---------- Posiciones iniciales ----------
const int START_PLAYER_X = 30;
const int START_PLAYER_Y = 30;
const int START_BOX_X = 60;
const int START_BOX_Y = 30;

// ---------- Variables de posición ----------
int playerX = START_PLAYER_X;
int playerY = START_PLAYER_Y;
int boxX = START_BOX_X;
int boxY = START_BOX_Y;

const int size = 10;
const int step = 10;

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
  bool moved = false;
  int dx = 0, dy = 0;

  if (digitalRead(BTN_UP) == LOW)    { dy = -step; moved = true; }
  if (digitalRead(BTN_DOWN) == LOW)  { dy = step;  moved = true; }
  if (digitalRead(BTN_LEFT) == LOW)  { dx = -step; moved = true; }
  if (digitalRead(BTN_RIGHT) == LOW) { dx = step;  moved = true; }

  if (digitalRead(BTN_START) == LOW) {
    clearSprite(playerX, playerY);
    clearSquare(boxX, boxY);
    playerX = START_PLAYER_X;
    playerY = START_PLAYER_Y;
    boxX = START_BOX_X;
    boxY = START_BOX_Y;
    drawScene();
    delay(300);
    return;
  }

  if (moved) {
    int newPlayerX = playerX + dx;
    int newPlayerY = playerY + dy;

    if (isColliding(newPlayerX, newPlayerY, boxX, boxY)) {
      int newBoxX = boxX + dx;
      int newBoxY = boxY + dy;

      if (isInside(newBoxX, newBoxY)) {
        clearSquare(boxX, boxY);
        boxX = newBoxX;
        boxY = newBoxY;

        clearSprite(playerX, playerY);
        playerX = newPlayerX;
        playerY = newPlayerY;
      }
    } else if (isInside(newPlayerX, newPlayerY)) {
      clearSprite(playerX, playerY);
      playerX = newPlayerX;
      playerY = newPlayerY;
    }

    drawScene();
    delay(100);
  }
}

// ---------- Dibujar escena ----------
void drawScene() {
  drawSnail(playerX, playerY);
  tft.fillRect(boxX, boxY, size, size, ST77XX_BLUE);
}

// ---------- Sprite caracol ----------
void drawSnail(int x, int y) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      uint16_t color = snailSprite[i][j];
      if (color != 0) {
        tft.drawPixel(x + j, y + i, color);
      }
    }
  }
}

void clearSprite(int x, int y) {
  tft.fillRect(x, y, size, size, ST77XX_BLACK);
}

void clearSquare(int x, int y) {
  tft.fillRect(x, y, size, size, ST77XX_BLACK);
}

bool isInside(int x, int y) {
  return (x >= 0 && x <= tft.width() - size && y >= 0 && y <= tft.height() - size);
}

bool isColliding(int ax, int ay, int bx, int by) {
  return (ax == bx && ay == by);
}