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

// ---------- Sprite Larry (10x10 ARGB32) ----------
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

// ---------- Posiciones iniciales ----------
const int START_PLAYER_X = 30;
const int START_PLAYER_Y = 30;
const int START_BOX_X = 60;
const int START_BOX_Y = 30;
const int GOAL_X = 100;
const int GOAL_Y = 30;

const int WALL_THICKNESS = 10;

// ---------- Variables de posición ----------
int playerX = START_PLAYER_X;
int playerY = START_PLAYER_Y;
int boxX = START_BOX_X;
int boxY = START_BOX_Y;

const int size = 10;
const int step = 5;
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

  bool moved = false;
  int dx = 0, dy = 0;

  if (digitalRead(BTN_UP) == LOW)    { dy = -step; moved = true; }
  if (digitalRead(BTN_DOWN) == LOW)  { dy = step;  moved = true; }
  if (digitalRead(BTN_LEFT) == LOW)  { dx = -step; moved = true; }
  if (digitalRead(BTN_RIGHT) == LOW) { dx = step;  moved = true; }

  if (digitalRead(BTN_START) == LOW) {
    clearSprite(playerX, playerY);
    clearSquare(boxX, boxY);
    tft.fillScreen(ST77XX_BLACK);
    playerX = START_PLAYER_X;
    playerY = START_PLAYER_Y;
    boxX = START_BOX_X;
    boxY = START_BOX_Y;
    victory = false;
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
        drawScene();
      }

    } else if (isInside(newPlayerX, newPlayerY)) {
      clearSprite(playerX, playerY);
      playerX = newPlayerX;
      playerY = newPlayerY;
      drawScene();
    }

    if (boxX == GOAL_X && boxY == GOAL_Y) {
      tft.setTextColor(ST77XX_WHITE);
      tft.setTextSize(2);
      tft.setCursor(20, 80);
      tft.println("VICTORIA!");
      victory = true;
    }

    delay(100);
  }
}

// ---------- Funciones auxiliares ----------
void drawScene() {
  // Zona meta
  tft.fillRect(GOAL_X, GOAL_Y, size, size, ST77XX_GREEN);

  // Paredes blancas alrededor
  tft.fillRect(0, 0, tft.width(), WALL_THICKNESS, ST77XX_WHITE); // superior
  tft.fillRect(0, tft.height() - WALL_THICKNESS, tft.width(), WALL_THICKNESS, ST77XX_WHITE); // inferior
  tft.fillRect(0, 0, WALL_THICKNESS, tft.height(), ST77XX_WHITE); // izquierda
  tft.fillRect(tft.width() - WALL_THICKNESS, 0, WALL_THICKNESS, tft.height(), ST77XX_WHITE); // derecha

  // Personaje y caja
  drawLarry(playerX, playerY);
  tft.fillRect(boxX, boxY, size, size, ST77XX_BLUE);
}

void drawLarry(int x, int y) {
  for (int row = 0; row < 10; row++) {
    for (int col = 0; col < 10; col++) {
      uint32_t color32 = larry_data[0][row * 10 + col];
      if ((color32 & 0xFFFFFF) != 0x000000) {
        uint8_t r = (color32 >> 16) & 0xFF;
        uint8_t g = (color32 >> 8) & 0xFF;
        uint8_t b = color32 & 0xFF;
        uint16_t color16 = tft.color565(r, g, b);
        tft.drawPixel(x + col, y + row, color16);
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
  return (
    x >= WALL_THICKNESS &&
    x <= tft.width() - WALL_THICKNESS - size &&
    y >= WALL_THICKNESS &&
    y <= tft.height() - WALL_THICKNESS - size
  );
}

bool isColliding(int ax, int ay, int bx, int by) {
  int margin = 4;
  return (
    abs(ax - bx) < size - margin &&
    abs(ay - by) < size - margin
  );
}