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
int playerY = SCREEN_HEIGHT - 20;

// Disparo del jugador
bool shotActive = false;
int shotX, shotY;

// Enemigos
const int numEnemies = 6;
int enemyWidth = 12;
int enemyHeight = 8;
int enemyX[numEnemies];
int enemyY[numEnemies];
bool enemyAlive[numEnemies];
int enemyDirection = 1; // 1 = derecha, -1 = izquierda

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
}

void loop() {
  tft.fillScreen(ST77XX_BLACK);

  readButtons();
  moveShot();
  moveEnemies();

  drawPlayer();
  drawShot();
  drawEnemies();

  delay(50); // Pequeño delay para estabilidad
}

// Inicializa la posición de los enemigos
void initEnemies() {
  int spacing = 18; // Espaciado horizontal
  for (int i = 0; i < numEnemies; i++) {
    enemyX[i] = 10 + i * spacing;
    enemyY[i] = 30; // Un poco más abajo
    enemyAlive[i] = true;
  }
}

// Dibuja la nave del jugador
void drawPlayer() {
  tft.fillRect(playerX, playerY, playerWidth, playerHeight, ST77XX_WHITE);
}

// Dibuja el disparo
void drawShot() {
  if (shotActive) {
    tft.fillRect(shotX, shotY, 3, 7, ST77XX_RED);
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

// Mueve el disparo hacia arriba
void moveShot() {
  if (shotActive) {
    shotY -= 5;
    if (shotY < 0) {
      shotActive = false;
    }
    // Detecta colisiones con enemigos
    for (int i = 0; i < numEnemies; i++) {
      if (enemyAlive[i] && 
          shotX >= enemyX[i] && shotX <= enemyX[i] + enemyWidth &&
          shotY >= enemyY[i] && shotY <= enemyY[i] + enemyHeight) {
        enemyAlive[i] = false;
        shotActive = false;
      }
    }
  }
}

// Mueve los enemigos de lado a lado
void moveEnemies() {
  for (int i = 0; i < numEnemies; i++) {
    enemyX[i] += enemyDirection;
  }
  // Cambiar dirección si tocan los bordes
  if (enemyX[0] < 0 || enemyX[numEnemies - 1] + enemyWidth > SCREEN_WIDTH) {
    enemyDirection = -enemyDirection;
    for (int i = 0; i < numEnemies; i++) {
      enemyY[i] += 10;
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
  if (digitalRead(BTN_START) == LOW && !shotActive) {
    shotX = playerX + playerWidth / 2;
    shotY = playerY;
    shotActive = true;
  }
}