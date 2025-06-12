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

// ---------- Cuadrado jugador ----------
int playerX = 30;
int playerY = 30;

// ---------- Cuadrado azul (empujable) ----------
int boxX = 60;
int boxY = 30;

const int size = 10;
const int step = 5;

void setup() {
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // Botones con pull-up
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  drawSquares();
}

void loop() {
  bool moved = false;
  int dx = 0, dy = 0;

  if (digitalRead(BTN_UP) == LOW)    { dy = -step; moved = true; }
  if (digitalRead(BTN_DOWN) == LOW)  { dy = step;  moved = true; }
  if (digitalRead(BTN_LEFT) == LOW)  { dx = -step; moved = true; }
  if (digitalRead(BTN_RIGHT) == LOW) { dx = step;  moved = true; }

  if (moved) {
    int newPlayerX = playerX + dx;
    int newPlayerY = playerY + dy;

    // Colisión con la caja azul
    bool collide = (newPlayerX == boxX && newPlayerY == boxY);

    // Intentar empujar
    if (collide) {
      int newBoxX = boxX + dx;
      int newBoxY = boxY + dy;

      if (isInside(newBoxX, newBoxY)) {
        clearSquare(boxX, boxY);
        boxX = newBoxX;
        boxY = newBoxY;
      } else {
        return; // no puede empujar
      }
    }

    if (isInside(newPlayerX, newPlayerY)) {
      clearSquare(playerX, playerY);
      playerX = newPlayerX;
      playerY = newPlayerY;
    }

    drawSquares();
    delay(100);
  }
}

void drawSquares() {
  tft.fillRect(playerX, playerY, size, size, ST77XX_WHITE);
  tft.fillRect(boxX, boxY, size, size, ST77XX_BLUE);
}

void clearSquare(int x, int y) {
  tft.fillRect(x, y, size, size, ST77XX_BLACK);
}

bool isInside(int x, int y) {
  return (x >= 0 && x <= tft.width() - size && y >= 0 && y <= tft.height() - size);
}