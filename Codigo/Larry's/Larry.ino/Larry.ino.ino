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
int x1 = 30;
int y1 = 30;

// ---------- Cuadrado azul (empujable) ----------
int x2 = 60;
int y2 = 30;

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
    int newX1 = x1 + dx;
    int newY1 = y1 + dy;

    // Verifica si colisiona con el cuadrado azul
    bool collide = (newX1 == x2 && newY1 == y2);

    // Si colisiona, intenta empujar el cuadrado azul
    if (collide) {
      int newX2 = x2 + dx;
      int newY2 = y2 + dy;

      // Solo mueve el azul si no sale de los límites
      if (isInside(newX2, newY2)) {
        clearSquare(x2, y2);
        x2 = newX2;
        y2 = newY2;
      } else {
        return; // no puede empujar, no se mueve
      }
    }

    // Verifica si el jugador puede moverse
    if (isInside(newX1, newY1)) {
      clearSquare(x1, y1);
      x1 = newX1;
      y1 = newY1;
    }

    drawSquares();
    delay(100);
  }
}

void drawSquares() {
  // jugador blanco
  tft.fillRect(x1, y1, size, size, ST77XX_WHITE);
  // empujable azul
  tft.fillRect(x2, y2, size, size, ST77XX_BLUE);
}

void clearSquare(int x, int y) {
  tft.fillRect(x, y, size, size, ST77XX_BLACK);
}

bool isInside(int x, int y) {
  return (x >= 0 && x <= tft.width() - size && y >= 0 && y <= tft.height() - size);
}