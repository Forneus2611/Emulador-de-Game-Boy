#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Definiciones pantalla ST7735
#define TFT_CS    10
#define TFT_DC    8
#define TFT_RST   9
#define TFT_SCLK  12
#define TFT_MOSI  11

SPIClass mySPI(HSPI);
Adafruit_ST7735 tft = Adafruit_ST7735(&mySPI, TFT_CS, TFT_DC, TFT_RST);

// Definiciones botones
#define BTN_UP     2
#define BTN_DOWN   3
#define BTN_LEFT   4
#define BTN_RIGHT  5
#define BTN_START  7
#define BTN_BACK   6

// Colores para cada dirección
#define COLOR_UP     ST77XX_BLUE
#define COLOR_RIGHT  ST77XX_YELLOW
#define COLOR_DOWN   ST77XX_GREEN
#define COLOR_LEFT   ST77XX_RED

// Variables
const int maxSequence = 100;
int sequence[maxSequence];
int currentLevel = 1;
bool playing = false;

void setup() {
  // Inicializar pantalla
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // Inicializar botones
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  // Mostrar pantalla de inicio
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 60);
  tft.print("SIMON GAME");
  tft.setTextSize(1);
  tft.setCursor(30, 100);
  tft.print("Press START");
}

void loop() {
  if (!playing) {
    if (digitalRead(BTN_START) == LOW) {
      delay(300); // Debounce
      startGame();
    }
  } else {
    showSequence();
    if (!playerTurn()) {
      gameOver();
    } else {
      currentLevel++;
      delay(500);
    }
  }
}

void startGame() {
  tft.fillScreen(ST77XX_BLACK);
  randomSeed(millis());
  for (int i = 0; i < maxSequence; i++) {
    sequence[i] = random(0, 4); // 0: Up, 1: Right, 2: Down, 3: Left
  }
  currentLevel = 1;
  playing = true;
}

void showSequence() {
  for (int i = 0; i < currentLevel; i++) {
    showColor(sequence[i]);
    delay(500);
    tft.fillScreen(ST77XX_BLACK);
    delay(200);
  }
}

void showColor(int dir) {
  switch (dir) {
    case 0: tft.fillScreen(COLOR_UP); break;
    case 1: tft.fillScreen(COLOR_RIGHT); break;
    case 2: tft.fillScreen(COLOR_DOWN); break;
    case 3: tft.fillScreen(COLOR_LEFT); break;
  }
}

bool playerTurn() {
  for (int i = 0; i < currentLevel; i++) {
    int input = waitForButton();
    if (input != sequence[i]) {
      return false;
    }
  }
  return true;
}

int waitForButton() {
  while (true) {
    if (digitalRead(BTN_UP) == LOW) {
      delay(200); // Debounce
      showColor(0);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 0;
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      delay(200);
      showColor(1);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 1;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      delay(200);
      showColor(2);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 2;
    }
    if (digitalRead(BTN_LEFT) == LOW) {
      delay(200);
      showColor(3);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 3;
    }
  }
}

void gameOver() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.print("GAME OVER");
  tft.setTextSize(1);
  tft.setCursor(10, 100);
  tft.print("Score: ");
  tft.print(currentLevel - 1);
  tft.setCursor(10, 120);
  tft.print("Press START");

  playing = false;
}

