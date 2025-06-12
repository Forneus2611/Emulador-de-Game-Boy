#include "main.h"

static int playerWidthInv = 8;
static int playerHeightInv = 8;
static int playerXInv, playerYInv;

static bool shotActive[INVADER_SHOTS];
static int shotX[INVADER_SHOTS];
static int shotY[INVADER_SHOTS];

static int enemyX[INVADER_ENEMIES];
static int enemyY[INVADER_ENEMIES];
static bool enemyAlive[INVADER_ENEMIES];
static int enemyDirection = 1;
static int enemySpeed = 1;

static int score = 0;
static int highScore = 0;
static bool gameOverInvaders = false;

enum InvadersState {
  MENU_INVADERS,
  PLAYING_INVADERS,
  GAMEOVER_INVADERS
};

static InvadersState invadersState = MENU_INVADERS;

static void initEnemiesInvaders() {
  int spacing = 18;
  for (int i = 0; i < INVADER_ENEMIES; i++) {
    enemyX[i] = 10 + i * spacing;
    enemyY[i] = 30;
    enemyAlive[i] = true;
  }
}

static void drawPlayerInvaders() {
  uint8_t ship[] = {
    0b00011100,
    0b00111110,
    0b01111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100
  };

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (ship[i] & (1 << (7 - j))) {
        tft.drawPixel(playerXInv + j, playerYInv + i, COLOR_LARRY);
      }
    }
  }
}

static void drawEnemyInvaders(int x, int y) {
  uint8_t alien[] = {
    0b00111100,
    0b01111110,
    0b11111111,
    0b11011011,
    0b11111111,
    0b00100100,
    0b01011010,
    0b10100101
  };

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (alien[i] & (1 << (7 - j))) {
        tft.drawPixel(x + j, y + i, ST77XX_GREEN);
      }
    }
  }
}

static void showMenuInvaders() {
  tft.fillScreen(COLOR_FONDO);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.println("SPACE");
  tft.setCursor(10, 70);
  tft.println("INVADERS");

  tft.setTextSize(1);
  tft.setCursor(20, 120);
  tft.println("Press Start");

  while (true) {
    if (digitalRead(BTN_START) == LOW) {
      invadersState = PLAYING_INVADERS;
      break;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      invadersState = MENU_INVADERS;
      estadoActual = STATE_MENU;
      drawMenu();
      delay(200);
      break;
    }
  }
}

static void moveEnemiesInvaders() {
  for (int i = 0; i < INVADER_ENEMIES; i++) {
    enemyX[i] += enemyDirection * enemySpeed;
  }
  if (enemyX[0] < 0 || enemyX[INVADER_ENEMIES - 1] + playerWidthInv > SCREEN_WIDTH) {
    enemyDirection = -enemyDirection;
    for (int i = 0; i < INVADER_ENEMIES; i++) {
      enemyY[i] += 10;
      if (enemyY[i] + playerHeightInv >= playerYInv) {
        gameOverInvaders = true;
      }
    }
  }
}

static void drawEnemiesInvaders() {
  for (int i = 0; i < INVADER_ENEMIES; i++) {
    if (enemyAlive[i]) {
      drawEnemyInvaders(enemyX[i], enemyY[i]);
    }
  }
}

static void moveShotsInvaders() {
  for (int i = 0; i < INVADER_SHOTS; i++) {
    if (shotActive[i]) {
      shotY[i] -= 5;
      if (shotY[i] < 0) {
        shotActive[i] = false;
      }
      for (int j = 0; j < INVADER_ENEMIES; j++) {
        if (enemyAlive[j] &&
            shotX[i] >= enemyX[j] && shotX[i] <= enemyX[j] + playerWidthInv &&
            shotY[i] >= enemyY[j] && shotY[i] <= enemyY[j] + playerHeightInv) {
          enemyAlive[j] = false;
          shotActive[i] = false;
          score++;
        }
      }
    }
  }
}

static void drawShotsInvaders() {
  for (int i = 0; i < INVADER_SHOTS; i++) {
    if (shotActive[i]) {
      tft.fillRect(shotX[i], shotY[i], 2, 6, ST77XX_RED);
    }
  }
}

static bool allEnemiesDeadInvaders() {
  for (int i = 0; i < INVADER_ENEMIES; i++) {
    if (enemyAlive[i]) {
      return false;
    }
  }
  return true;
}

static void nextWaveInvaders() {
  enemySpeed += 1;
  initEnemiesInvaders();
}

static void readButtonsInvaders() {
  if (digitalRead(BTN_LEFT) == LOW && playerXInv > 0) {
    playerXInv -= 4;
  }
  if (digitalRead(BTN_RIGHT) == LOW && playerXInv < (SCREEN_WIDTH - playerWidthInv)) {
    playerXInv += 4;
  }
  if (digitalRead(BTN_START) == LOW) {
    for (int i = 0; i < INVADER_SHOTS; i++) {
      if (!shotActive[i]) {
        shotX[i] = playerXInv + playerWidthInv / 2;
        shotY[i] = playerYInv;
        shotActive[i] = true;
        break;
      }
    }
  }
}

static void drawScoreInvaders() {
  tft.setCursor(2, 2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.print("Score: ");
  tft.print(score);
  tft.setCursor(80, 2);
  tft.print("High: ");
  tft.print(highScore);
}

static void showGameOverInvaders() {
  if (score > highScore) {
    highScore = score;
  }

  tft.fillScreen(COLOR_FONDO);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.setCursor(20, 40);
  tft.println("GAME OVER");

  tft.setTextSize(1);
  tft.setCursor(20, 80);
  tft.print("Score: ");
  tft.println(score);

  tft.setCursor(20, 100);
  tft.print("High Score: ");
  tft.println(highScore);

  tft.setCursor(20, 130);
  tft.println("Start=Retry");
  tft.setCursor(20, 140);
  tft.println("Back=Menu");

  while (true) {
    if (digitalRead(BTN_START) == LOW) {
      playerXInv = SCREEN_WIDTH / 2 - playerWidthInv / 2;
      playerYInv = SCREEN_HEIGHT - 40;
      enemySpeed = 1;
      score = 0;
      for (int i = 0; i < INVADER_SHOTS; i++) shotActive[i] = false;
      initEnemiesInvaders();
      gameOverInvaders = false;
      invadersState = PLAYING_INVADERS;
      break;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      invadersState = MENU_INVADERS;
      estadoActual = STATE_MENU;
      drawMenu();
      delay(200);
      break;
    }
  }
}

void invadersGame() {
  static bool invadersInitialized = false;
  if (!invadersInitialized) {
    playerXInv = SCREEN_WIDTH / 2 - playerWidthInv / 2;
    playerYInv = SCREEN_HEIGHT - 40;
    enemySpeed = 1;
    score = 0;
    for (int i = 0; i < INVADER_SHOTS; i++) shotActive[i] = false;
    initEnemiesInvaders();
    gameOverInvaders = false;
    invadersState = MENU_INVADERS;
    invadersInitialized = true;
  }

  if (invadersState == MENU_INVADERS) {
    showMenuInvaders();
  } else if (invadersState == PLAYING_INVADERS) {
    tft.fillScreen(COLOR_FONDO);
    readButtonsInvaders();
    moveShotsInvaders();
    moveEnemiesInvaders();
    drawPlayerInvaders();
    drawShotsInvaders();
    drawEnemiesInvaders();
    drawScoreInvaders();

    if (allEnemiesDeadInvaders()) {
      nextWaveInvaders();
    }

    if (gameOverInvaders) {
      invadersState = GAMEOVER_INVADERS;
    }

    delay(50);
  } else if (invadersState == GAMEOVER_INVADERS) {
    showGameOverInvaders();
  }
}
