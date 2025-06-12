#include "main.h"

// Forward declarations
static void showColorSimon(int dir);
static int waitForButtonSimon();



static int sequence[SIMON_MAX_SEQ];
static int currentLevelSimon = 1;
static bool playingSimon = false;
static bool gameOverSimon = false;

static void showStartScreenSimon() {
  tft.fillScreen(COLOR_FONDO);
  tft.setTextColor(COLOR_LARRY);
  tft.setTextSize(2);
  tft.setCursor(20, 60);
  tft.println("SIMON GAME");
  tft.setTextSize(1);
  tft.setCursor(30, 100);
  tft.println("Press START");
}

static void showSequenceSimon() {
  for (int i = 0; i < currentLevelSimon; i++) {
    showColorSimon(sequence[i]);
    delay(500);
    tft.fillScreen(COLOR_FONDO);
    delay(200);
  }
}

static void showColorSimon(int dir) {
  switch (dir) {
    case 0: tft.fillScreen(SIMON_COLOR_UP); break;
    case 1: tft.fillScreen(SIMON_COLOR_RIGHT); break;
    case 2: tft.fillScreen(SIMON_COLOR_DOWN); break;
    case 3: tft.fillScreen(SIMON_COLOR_LEFT); break;
  }
}

static bool playerTurnSimon() {
  for (int i = 0; i < currentLevelSimon; i++) {
    int input = waitForButtonSimon();
    if (input != sequence[i]) {
      return false;
    }
  }
  return true;
}

static int waitForButtonSimon() {
  while (true) {
    if (digitalRead(BTN_UP) == LOW) {
      delay(200);
      showColorSimon(0);
      delay(300);
      tft.fillScreen(COLOR_FONDO);
      return 0;
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      delay(200);
      showColorSimon(1);
      delay(300);
      tft.fillScreen(COLOR_FONDO);
      return 1;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      delay(200);
      showColorSimon(2);
      delay(300);
      tft.fillScreen(COLOR_FONDO);
      return 2;
    }
    if (digitalRead(BTN_LEFT) == LOW) {
      delay(200);
      showColorSimon(3);
      delay(300);
      tft.fillScreen(COLOR_FONDO);
      return 3;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      playingSimon = false;
      gameOverSimon = false;
      estadoActual = STATE_MENU;
      drawMenu();
      delay(200);
      return -1;
    }
  }
}

static void gameOverScreenSimon() {
  playingSimon = false;
  gameOverSimon = true;
  tft.fillScreen(COLOR_FONDO);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.println("GAME OVER");
  tft.setTextSize(1);
  tft.setCursor(10, 100);
  tft.print("Score: ");
  tft.print(currentLevelSimon - 1);
  tft.setCursor(10, 120);
  tft.println("Press BACK");
  while (true) {
    if (digitalRead(BTN_START) == LOW) {
      currentLevelSimon = 1;
      playingSimon = false;
      gameOverSimon = false;
      randomSeed(millis());
      for (int i = 0; i < SIMON_MAX_SEQ; i++) {
        sequence[i] = random(0, 4);
      }
      showStartScreenSimon();
      delay(300);
      break;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      playingSimon = false;
      gameOverSimon = false;
      estadoActual = STATE_MENU;
      drawMenu();
      delay(300);
      break;
    }
  }
}

void simonGame() {
  static bool simonInitialized = false;
  if (!simonInitialized) {
    randomSeed(millis());
    for (int i = 0; i < SIMON_MAX_SEQ; i++) {
      sequence[i] = random(0, 4);
    }
    showStartScreenSimon();
    simonInitialized = true;
  }

  if (!playingSimon && !gameOverSimon) {
    if (digitalRead(BTN_START) == LOW) {
      delay(300);
      playingSimon = true;
    }
  } else if (playingSimon) {
    showSequenceSimon();
    if (!playerTurnSimon()) {
      gameOverScreenSimon();
    } else {
      currentLevelSimon++;
      delay(500);
    }
  }
}
