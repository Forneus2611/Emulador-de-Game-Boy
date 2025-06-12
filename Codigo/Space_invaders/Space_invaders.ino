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

// Nave del jugador
int playerX = 64;
int playerY = 140;

// Disparo del jugador
bool shotActive = false;
int shotX, shotY;

// Enemigos
const int numEnemies = 5;
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