#include "main.h"

// Variables globales
SPIClass mySPI(HSPI);
Adafruit_ST7735 tft = Adafruit_ST7735(&mySPI, TFT_CS, TFT_DC, TFT_RST);

int estadoActual = STATE_MENU;
unsigned long lastDebounceTime = 0;

const int totalGames = 3;
const char* gameList[] = {
  "Larry's Adventures",
  "Simon Dice",
  "Space Invaders"
};

void inicializarSistema() {
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(COLOR_FONDO);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  drawMenu();
}

void ejecutarLoop() {
  switch (estadoActual) {
    case STATE_MENU:
      menuLoop();
      break;
    case STATE_LARRY:
      larryGame();
      break;
    case STATE_SIMON:
      simonGame();
      break;
    case STATE_INVADERS:
      invadersGame();
      break;
  }
}

void menuLoop() {
  static int currentSelection = 0;
  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_UP) == LOW) {
      currentSelection = (currentSelection - 1 + totalGames) % totalGames;
      drawMenu();
      lastDebounceTime = millis();
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      currentSelection = (currentSelection + 1) % totalGames;
      drawMenu();
      lastDebounceTime = millis();
    }
    if (digitalRead(BTN_START) == LOW) {
      estadoActual = currentSelection + 1;
      tft.fillScreen(COLOR_FONDO);
      lastDebounceTime = millis();
    }
  }
}

void drawMenu() {
  tft.fillScreen(COLOR_FONDO);
  tft.setTextSize(2);
  tft.setTextColor(COLOR_LARRY);
  tft.setCursor(10, 10);
  tft.println("Menu de Juegos");

  for (int i = 0; i < totalGames; i++) {
    if (i == 0) tft.setTextColor(ST77XX_YELLOW);
    else tft.setTextColor(COLOR_LARRY);
    tft.setCursor(20, 40 + i * 30);
    tft.println(gameList[i]);
  }
}
