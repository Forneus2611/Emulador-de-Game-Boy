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

// Definiciones generales
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160

// Nave del jugador
int playerWidth = 14;
int playerHeight = 8;
int playerX = SCREEN_WIDTH / 2 - playerWidth / 2;
int playerY = SCREEN_HEIGHT - 40; // Nave más arriba

// Disparos múltiples
#define MAX_SHOTS 5
bool shotActive[MAX_SHOTS];
int shotX[MAX_SHOTS];
int shotY[MAX_SHOTS];

// Enemigos
const int numEnemies = 6;
int enemyWidth = 12;
int enemyHeight = 8;
int enemyX[numEnemies];
int enemyY[numEnemies];
bool enemyAlive[numEnemies];
int enemyDirection = 1; // 1 = derecha, -1 = izquierda

// Velocidad de enemigos
int enemySpeed = 1;

// Game Over flag
bool gameOver = false;

void setup() {
  // Inicializar botones
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  // Inicializar pantalla
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1); // Horizontal
  tft.fillScreen(ST77XX_BLACK);

  // Inicializar enemigos
  initEnemies();

  // Inicializar disparos
  for (int i = 0; i < MAX_SHOTS; i++) {
    shotActive[i] = false;
  }
}

void loop() {
  if (gameOver) { 
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(20, SCREEN_HEIGHT / 2);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);
    tft.println("GAME OVER");
    while (true); // Detener el juego
  }

  tft.fillScreen(ST77XX_BLACK);

  readButtons();
  moveShot();
  moveEnemies();

  drawPlayer();
  drawShot();
  drawEnemies();

  if (allEnemiesDead()) {
    nextWave();
  }

  delay(50);
}

// Inicializa la posición de los enemigos
void initEnemies() {
  int spacing = 18; // Espaciado horizontal
  for (int i = 0; i < numEnemies; i++) {
    enemyX[i] = 10 + i * spacing;
    enemyY[i] = 30;
    enemyAlive[i] = true;
  }
}

// Dibuja la nave del jugador
void drawPlayer() {
  tft.fillRect(playerX, playerY, playerWidth, playerHeight, ST77XX_WHITE);
}

// Dibuja múltiples disparos
void drawShot() {
  for (int i = 0; i < MAX_SHOTS; i++) {
    if (shotActive[i]) {
      tft.fillRect(shotX[i], shotY[i], 3, 7, ST77XX_RED);
    }
  }
}

// Mueve múltiples disparos
void moveShot() {
  for (int i = 0; i < MAX_SHOTS; i++) {
    if (shotActive[i]) {
      shotY[i] -= 5;
      if (shotY[i] < 0) {
        shotActive[i] = false;
      }
      // Detecta colisiones con enemigos
      for (int j = 0; j < numEnemies; j++) {
        if (enemyAlive[j] && 
            shotX[i] >= enemyX[j] && shotX[i] <= enemyX[j] + enemyWidth &&
            shotY[i] >= enemyY[j] && shotY[i] <= enemyY[j] + enemyHeight) {
          enemyAlive[j] = false;
          shotActive[i] = false;
        }
      }
    }
  }
}

// Dibuja los enemigos
void drawEnemies() {
  for (int i = 0; i < numEnemies; i++) {
    if (enemyAlive[i]) {
      tft.fillRect(enemyX[i], enemyY[i], enemyWidth, enemyHeight, ST77XX_GREEN);
    }
  }
}

// Mueve los enemigos de lado a lado
void moveEnemies() {
  for (int i = 0; i < numEnemies; i++) {
    enemyX[i] += enemyDirection * enemySpeed;
  }
  if (enemyX[0] < 0 || enemyX[numEnemies - 1] + enemyWidth > SCREEN_WIDTH) {
    enemyDirection = -enemyDirection;
    for (int i = 0; i < numEnemies; i++) {
      enemyY[i] += 10;
      if (enemyY[i] + enemyHeight >= playerY) {
        gameOver = true;
      }
    }
  }
}

// Lee los botones para mover la nave o disparar
void readButtons() {
  if (digitalRead(BTN_LEFT) == LOW && playerX > 0) {
    playerX -= 4;
  }
  if (digitalRead(BTN_RIGHT) == LOW && playerX < (SCREEN_WIDTH - playerWidth)) {
    playerX += 4;
  }
  if (digitalRead(BTN_START) == LOW) {
    for (int i = 0; i < MAX_SHOTS; i++) {
      if (!shotActive[i]) {
        shotX[i] = playerX + playerWidth / 2;
        shotY[i] = playerY;
        shotActive[i] = true;
        break;
      }
    }
  }
}

// Verifica si todos los enemigos están muertos
bool allEnemiesDead() {
  for (int i = 0; i < numEnemies; i++) {
    if (enemyAlive[i]) {
      return false;
    }
  }
  return true;
}

// Siguiente ola
void nextWave() {
  enemySpeed += 1;
  initEnemies();
}