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

// ---------- Propiedades del "jugador" ----------
int x = 60;          // posición inicial X
int y = 60;          // posición inicial Y
const int size = 10; // tamaño del cuadrado
const int step = 5;  // cantidad de movimiento por pulsación

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

  drawPlayer();
}

void loop() {
  bool moved = false;

  // Detectar botones y mover
  if (digitalRead(BTN_UP) == LOW && y > 0) {
    clearPlayer();
    y -= step;
    moved = true;
  }
  if (digitalRead(BTN_DOWN) == LOW && y < tft.height() - size) {
    clearPlayer();
    y += step;
    moved = true;
  }
  if (digitalRead(BTN_LEFT) == LOW && x > 0) {
    clearPlayer();
    x -= step;
    moved = true;
  }
  if (digitalRead(BTN_RIGHT) == LOW && x < tft.width() - size) {
    clearPlayer();
    x += step;
    moved = true;
  }

  if (moved) {
    drawPlayer();
    delay(100); // para evitar rebotes
  }
}

// ---------- Funciones de dibujo ----------
void drawPlayer() {
  tft.fillRect(x, y, size, size, ST77XX_WHITE);
}

void clearPlayer() {
  tft.fillRect(x, y, size, size, ST77XX_BLACK);
}