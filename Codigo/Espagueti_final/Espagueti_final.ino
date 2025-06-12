#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Pines pantalla TFT
#define TFT_CS    10
#define TFT_DC    8
#define TFT_RST   9
#define TFT_SCLK  12
#define TFT_MOSI  11

SPIClass mySPI(HSPI);
Adafruit_ST7735 tft = Adafruit_ST7735(&mySPI, TFT_CS, TFT_DC, TFT_RST);

#define BTN_UP     2
#define BTN_DOWN   3
#define BTN_LEFT   4
#define BTN_RIGHT  5
#define BTN_BACK   6
#define BTN_START  7

#define STATE_MENU      0
#define STATE_LARRY     1
#define STATE_SIMON     2
#define STATE_INVADERS  3

int state = STATE_MENU;
int currentSelection = 0;
const int totalGames = 3;


const char* gameList[] = {
  "Larry's Adventures",
  "Simon Dice",
  "Space Invaders"
};


unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;


bool larryInitialized = false;
bool simonInitialized = false;
bool invadersInitialized = false;

void setup() {
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  drawMenu();
}

void loop() {
  switch (state) {
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
  if (millis() - lastDebounceTime > debounceDelay) {
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
      state = currentSelection + 1;
      tft.fillScreen(ST77XX_BLACK);
      lastDebounceTime = millis();
    }
  }
}

void drawMenu() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.println("Menu de Juegos");

  for (int i = 0; i < totalGames; i++) {
    if (i == currentSelection) {
      tft.setTextColor(ST77XX_YELLOW);
    } else {
      tft.setTextColor(ST77XX_WHITE);
    }
    tft.setCursor(20, 40 + i * 30);
    tft.println(gameList[i]);
  }
}

const int size = 10;
const int step = 10;
const int MAX_CAJAS = 5;
int larryX, larryY;  // playerX -> larryX
int boxX[MAX_CAJAS], boxY[MAX_CAJAS];
int goalX[MAX_CAJAS], goalY[MAX_CAJAS];
int cajasEncontradas = 0;
int metasEncontradas = 0;
bool victory = false;
bool pantallaInicioMostrada = false;
unsigned long lastLarryDebounce = 0;
const unsigned long debounceDelayLarry = 150;
int nivelActual = 0;


static const uint32_t larry_data[1][100] = {{
  0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xff000000,
  0xff000000, 0xffffff00, 0xff000000, 0xffffff00, 0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xffffff00, 0xff000000,
  0xff000000, 0xffffffff, 0xffffff00, 0xffffff00, 0xff000000, 0xff000000, 0xffffffff, 0xffffff00, 0xffffff00, 0xff000000,
  0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffff00, 0xff000000, 0xff000000,
  0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xff000000, 0xffffff00, 0xffffff00, 0xff000000, 0xff000000, 0xff000000, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00,
  0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00
}};

static const uint32_t boxSprite[1][100] = {{
  0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513,
  0xff8b4513, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xffa0522d, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xffa0522d, 0xff8b4513,
  0xff8b4513, 0xff000000, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xffa0522d, 0xff000000, 0xff8b4513,
  0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513, 0xff8b4513
}};

const char nivel1[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','2','2','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','L','2','M','C','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','2','2','C','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','1','M','2','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','2','2','2','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel2[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','2','2','2','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','2','2','M','M','2','1','1','1','1','1'},
  {'1','1','1','1','1','2','C','C','C','L','2','1','1','1','1','1'},
  {'1','1','1','1','1','2','2','1','2','M','2','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel3[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','L','M','C','2','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','C','1','2','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','C','2','M','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','M','2','2','2','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel4[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','2','2','2','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','2','M','M','2','2','2','1','1','1','1','1'},
  {'1','1','1','1','1','2','1','2','C','C','C','1','1','1','1','1'},
  {'1','1','1','1','1','2','2','2','1','L','M','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel5[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','2','2','M','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','2','1','M','2','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','2','2','M','2','2','1','1','1','1','1'},
  {'1','1','1','1','1','1','2','C','C','C','2','1','1','1','1','1'},
  {'1','1','1','1','1','1','2','2','1','2','L','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char (*niveles[])[16] = { nivel1, nivel2, nivel3, nivel4, nivel5 };
const int TOTAL_NIVELES = sizeof(niveles) / sizeof(niveles[0]);
const char (*nivel)[16];

void pantallaInicioLarry() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.println("Larry's Adventures");
  tft.setTextSize(1);
  tft.setCursor(20, 80);
  tft.println("Presiona START");
}

void pantallaNivelLarry(int numeroNivel) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, 40);
  tft.print("Nivel ");
  tft.println(numeroNivel);
  delay(1500);
}

void drawLarry(int x, int y) {
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      uint32_t color32 = larry_data[0][r * size + c];
      if ((color32 & 0xFFFFFF) != 0x000000) {
        uint16_t color16 = tft.color565((color32 >> 16) & 0xFF, (color32 >> 8) & 0xFF, color32 & 0xFF);
        tft.drawPixel(x + c, y + r, color16);
      }
    }
  }
}

void drawBox(int x, int y) {
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      uint32_t color32 = boxSprite[0][r * size + c];
      if ((color32 & 0xFFFFFF) != 0x000000) {
        uint16_t color16 = tft.color565((color32 >> 16) & 0xFF, (color32 >> 8) & 0xFF, color32 & 0xFF);
        tft.drawPixel(x + c, y + r, color16);
      }
    }
  }
}

void clearSprite(int x, int y) {
  int cx = x / size;
  int cy = y / size;
  char ch = nivel[cy][cx];
  if (ch == '1') tft.fillRect(x, y, size, size, tft.color565(100, 100, 100));
  else if (ch == 'M') tft.fillRect(x, y, size, size, ST77XX_GREEN);
  else tft.fillRect(x, y, size, size, ST77XX_BLACK);
}

bool isWall(int x, int y) {
  int cx = x / size;
  int cy = y / size;
  return nivel[cy][cx] == '1';
}

bool isInside(int x, int y) {
  return x >= 0 && x <= tft.width() - size && y >= 0 && y <= tft.height() - size;
}

bool isColliding(int ax, int ay, int bx, int by) {
  int margin = 4;
  return abs(ax - bx) < size - margin && abs(ay - by) < size - margin;
}

void drawScene() {
  tft.fillScreen(ST77XX_BLACK); // 👈 BORRAR TODO ANTES
  for (int y = 0; y < 13; y++) {
    for (int x = 0; x < 16; x++) {
      int px = x * size;
      int py = y * size;
      char ch = nivel[y][x];
      if (ch == '1') tft.fillRect(px, py, size, size, tft.color565(100, 100, 100));
      else if (ch == 'M') tft.fillRect(px, py, size, size, ST77XX_GREEN);
    }
  }
  for (int i = 0; i < metasEncontradas; i++) {
    bool cajaEnMeta = false;
    for (int j = 0; j < cajasEncontradas; j++) {
      if (boxX[j] == goalX[i] && boxY[j] == goalY[i]) {
        cajaEnMeta = true;
        break;
      }
    }
    tft.fillRect(goalX[i], goalY[i], size, size, cajaEnMeta ? ST77XX_BLUE : ST77XX_GREEN);
  }
  for (int i = 0; i < cajasEncontradas; i++) {
    drawBox(boxX[i], boxY[i]);
  }
  drawLarry(larryX, larryY);
}

void setupNivel() {
  cajasEncontradas = 0;
  metasEncontradas = 0;
  
  tft.fillScreen(ST77XX_BLACK);
  
  for (int y = 0; y < 13; y++) {
    for (int x = 0; x < 16; x++) {
      int px = x * size;
      int py = y * size;
      char ch = nivel[y][x];
      if (ch == '1') tft.fillRect(px, py, size, size, tft.color565(100, 100, 100));
      else if (ch == 'M') {
        tft.fillRect(px, py, size, size, ST77XX_GREEN);
        goalX[metasEncontradas] = px;
        goalY[metasEncontradas] = py;
        metasEncontradas++;
      }
      else if (ch == 'L') {
        larryX = px; larryY = py;
      }
      else if (ch == 'C') {
        boxX[cajasEncontradas] = px;
        boxY[cajasEncontradas] = py;
        cajasEncontradas++;
      }
    }
  }
  drawScene();
}

void initLarryGame() {
  pantallaInicioMostrada = false;
  victory = false;
  nivelActual = 0;
  nivel = niveles[nivelActual];
  pantallaInicioLarry();
}

void larryGame() {
  if (!larryInitialized) {
    initLarryGame();
    larryInitialized = true;
  }

  if (!pantallaInicioMostrada) {
    if (digitalRead(BTN_START) == LOW) {
      pantallaInicioMostrada = true;
      nivelActual = 0;
      nivel = niveles[nivelActual];
      pantallaNivelLarry(nivelActual + 1);
      setupNivel();
      delay(500);
    }
    return;
  }

  if (victory) {
    if (digitalRead(BTN_BACK) == LOW) {
      larryInitialized = false;
      state = STATE_MENU;
      drawMenu();
      delay(300);
    }
    return;
  }

  int dx = 0, dy = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastLarryDebounce > debounceDelayLarry) {
    if (digitalRead(BTN_UP) == LOW) { dy = -step; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_DOWN) == LOW) { dy = step; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_LEFT) == LOW) { dx = -step; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_RIGHT) == LOW) { dx = step; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_START) == LOW) {
      setupNivel();
      lastLarryDebounce = currentTime;
      return;
    }
  }

  if (dx != 0 || dy != 0) {   // 👈 SOLO si te mueves
    int newLarryX = larryX + dx;
    int newLarryY = larryY + dy;

    if (!isInside(newLarryX, newLarryY) || isWall(newLarryX, newLarryY)) return;

    bool empuja = false;
    for (int i = 0; i < cajasEncontradas; i++) {
      if (isColliding(newLarryX, newLarryY, boxX[i], boxY[i])) {
        int newBoxX = boxX[i] + dx;
        int newBoxY = boxY[i] + dy;

        bool colisionOtraCaja = false;
        for (int j = 0; j < cajasEncontradas; j++) {
          if (j != i && isColliding(newBoxX, newBoxY, boxX[j], boxY[j])) {
            colisionOtraCaja = true;
            break;
          }
        }

        if (isInside(newBoxX, newBoxY) && !isWall(newBoxX, newBoxY) && !colisionOtraCaja) {
          clearSprite(boxX[i], boxY[i]);     
          boxX[i] = newBoxX;
          boxY[i] = newBoxY;
          drawBox(boxX[i], boxY[i]);         

          clearSprite(larryX, larryY);       
          larryX = newLarryX;
          larryY = newLarryY;
          drawLarry(larryX, larryY);         
          empuja = true;
          break;
        } else {
          return;
        }
      }
    }

    if (!empuja) {
      clearSprite(larryX, larryY);       
      larryX = newLarryX;
      larryY = newLarryY;
      drawLarry(larryX, larryY);          
    }

    bool victoriaActual = true;
    for (int i = 0; i < metasEncontradas; i++) {
      bool metaOcupada = false;
      for (int j = 0; j < cajasEncontradas; j++) {
        if (boxX[j] == goalX[i] && boxY[j] == goalY[i]) {
          metaOcupada = true;
          break;
        }
      }
      if (!metaOcupada) {
        victoriaActual = false;
        break;
      }
    }

    if (victoriaActual) {
      tft.setTextColor(ST77XX_WHITE);
      tft.setTextSize(2);
      tft.setCursor(20, 80);
      tft.println("VICTORIA!");
      delay(1000);

      nivelActual++;
      if (nivelActual < TOTAL_NIVELES) {
        nivel = niveles[nivelActual];
        pantallaNivelLarry(nivelActual + 1);
        setupNivel();
      } else {
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(10, 40);
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_GREEN);
        tft.println("JUEGO COMPLETADO!");
        victory = true;
      }
    }
  }

  if (digitalRead(BTN_BACK) == LOW) {
    larryInitialized = false;
    state = STATE_MENU;
    drawMenu();
    delay(200);
  }
}





#define COLOR_UP     ST77XX_BLUE
#define COLOR_RIGHT  ST77XX_YELLOW
#define COLOR_DOWN   ST77XX_GREEN
#define COLOR_LEFT   ST77XX_RED

const int maxSequence = 100;
int sequence[maxSequence];
int currentLevelSimon = 1;
bool playingSimon = false;
bool gameOverSimon = false;

void initSimonGame() {
  tft.fillScreen(ST77XX_BLACK);
  randomSeed(millis());
  for (int i = 0; i < maxSequence; i++) {
    sequence[i] = random(0, 4); 
  }
  currentLevelSimon = 1;
  playingSimon = false;
  gameOverSimon = false;
  showStartScreenSimon();
}

void showStartScreenSimon() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 60);
  tft.println("SIMON GAME");
  tft.setTextSize(1);
  tft.setCursor(30, 100);
  tft.println("Press START");
}

void showSequenceSimon() {
  for (int i = 0; i < currentLevelSimon; i++) {
    showColorSimon(sequence[i]);
    delay(500);
    tft.fillScreen(ST77XX_BLACK);
    delay(200);
  }
}

void showColorSimon(int dir) {
  switch (dir) {
    case 0: tft.fillScreen(COLOR_UP); break;
    case 1: tft.fillScreen(COLOR_RIGHT); break;
    case 2: tft.fillScreen(COLOR_DOWN); break;
    case 3: tft.fillScreen(COLOR_LEFT); break;
  }
}

bool playerTurnSimon() {
  for (int i = 0; i < currentLevelSimon; i++) {
    int input = waitForButtonSimon();
    if (input != sequence[i]) {
      return false;
    }
  }
  return true;
}

int waitForButtonSimon() {
  while (true) {
    if (digitalRead(BTN_UP) == LOW) {
      delay(200);
      showColorSimon(0);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 0;
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      delay(200);
      showColorSimon(1);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 1;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      delay(200);
      showColorSimon(2);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 2;
    }
    if (digitalRead(BTN_LEFT) == LOW) {
      delay(200);
      showColorSimon(3);
      delay(300);
      tft.fillScreen(ST77XX_BLACK);
      return 3;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      simonInitialized = false;
      state = STATE_MENU;
      drawMenu();
      delay(200);
      return -1;
    }
  }
}

void gameOverScreenSimon() {
  playingSimon = false;
  gameOverSimon = true;
  tft.fillScreen(ST77XX_BLACK);
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
      initSimonGame();    
      simonInitialized = true;
      playingSimon = false;
      gameOverSimon = false;
      delay(300);
      break;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      simonInitialized = false;
      state = STATE_MENU;
      drawMenu();
      delay(300);
      break;
    }
  }
}

void simonGame() {
  if (!simonInitialized) {
    initSimonGame();
    simonInitialized = true;
  }

  if (!playingSimon && !gameOverSimon) {
    if (digitalRead(BTN_START) == LOW) {
      delay(300);
      playingSimon = true;
    }
  } 
  else if (playingSimon) {
    showSequenceSimon();
    if (!playerTurnSimon()) {
      gameOverScreenSimon();
    } else {
      currentLevelSimon++;
      delay(500);
    }
  }
}

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160

int playerWidthInv = 8;
int playerHeightInv = 8;
int playerXInv, playerYInv;

#define MAX_SHOTS 5
bool shotActive[MAX_SHOTS];
int shotX[MAX_SHOTS];
int shotY[MAX_SHOTS];

const int numEnemies = 6;
int enemyWidth = 8;
int enemyHeight = 8;
int enemyX[numEnemies];
int enemyY[numEnemies];
bool enemyAlive[numEnemies];
int enemyDirection = 1;
int enemySpeed = 1;

int score = 0;
int highScore = 0;

bool gameOverInvaders = false;

enum InvadersState {
  MENU_INVADERS,
  PLAYING_INVADERS,
  GAMEOVER_INVADERS
};

InvadersState invadersState = MENU_INVADERS;

void initInvadersGame() {
  playerXInv = SCREEN_WIDTH / 2 - playerWidthInv / 2;
  playerYInv = SCREEN_HEIGHT - 40;
  enemySpeed = 1;
  score = 0;
  for (int i = 0; i < MAX_SHOTS; i++) {
    shotActive[i] = false;
  }
  initEnemiesInvaders();
  gameOverInvaders = false;
  invadersState = MENU_INVADERS;
}

void initEnemiesInvaders() {
  int spacing = 18;
  for (int i = 0; i < numEnemies; i++) {
    enemyX[i] = 10 + i * spacing;
    enemyY[i] = 30;
    enemyAlive[i] = true;
  }
}

void drawPlayerInvaders() {
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
        tft.drawPixel(playerXInv + j, playerYInv + i, ST77XX_WHITE);
      }
    }
  }
}

void drawEnemyInvaders(int x, int y) {
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

void showMenuInvaders() {
  tft.fillScreen(ST77XX_BLACK);
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
      invadersInitialized = false;
      state = STATE_MENU;
      drawMenu();
      delay(200);
      break;
    }
  }
}

void moveEnemiesInvaders() {
  for (int i = 0; i < numEnemies; i++) {
    enemyX[i] += enemyDirection * enemySpeed;
  }
  if (enemyX[0] < 0 || enemyX[numEnemies - 1] + enemyWidth > SCREEN_WIDTH) {
    enemyDirection = -enemyDirection;
    for (int i = 0; i < numEnemies; i++) {
      enemyY[i] += 10;
      if (enemyY[i] + enemyHeight >= playerYInv) {
        gameOverInvaders = true;
      }
    }
  }
}

void drawEnemiesInvaders() {
  for (int i = 0; i < numEnemies; i++) {
    if (enemyAlive[i]) {
      drawEnemyInvaders(enemyX[i], enemyY[i]);
    }
  }
}

void moveShotsInvaders() {
  for (int i = 0; i < MAX_SHOTS; i++) {
    if (shotActive[i]) {
      shotY[i] -= 5;
      if (shotY[i] < 0) {
        shotActive[i] = false;
      }
      for (int j = 0; j < numEnemies; j++) {
        if (enemyAlive[j] &&
            shotX[i] >= enemyX[j] && shotX[i] <= enemyX[j] + enemyWidth &&
            shotY[i] >= enemyY[j] && shotY[i] <= enemyY[j] + enemyHeight) {
          enemyAlive[j] = false;
          shotActive[i] = false;
          score++;
        }
      }
    }
  }
}

void drawShotsInvaders() {
  for (int i = 0; i < MAX_SHOTS; i++) {
    if (shotActive[i]) {
      tft.fillRect(shotX[i], shotY[i], 2, 6, ST77XX_RED);
    }
  }
}

bool allEnemiesDeadInvaders() {
  for (int i = 0; i < numEnemies; i++) {
    if (enemyAlive[i]) {
      return false;
    }
  }
  return true;
}

void nextWaveInvaders() {
  enemySpeed += 1;
  initEnemiesInvaders();
}

void readButtonsInvaders() {
  if (digitalRead(BTN_LEFT) == LOW && playerXInv > 0) {
    playerXInv -= 4;
  }
  if (digitalRead(BTN_RIGHT) == LOW && playerXInv < (SCREEN_WIDTH - playerWidthInv)) {
    playerXInv += 4;
  }
  if (digitalRead(BTN_START) == LOW) {
    for (int i = 0; i < MAX_SHOTS; i++) {
      if (!shotActive[i]) {
        shotX[i] = playerXInv + playerWidthInv / 2;
        shotY[i] = playerYInv;
        shotActive[i] = true;
        break;
      }
    }
  }
}

void drawScoreInvaders() {
  tft.setCursor(2, 2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.print("Score: ");
  tft.print(score);
  tft.setCursor(80, 2);
  tft.print("High: ");
  tft.print(highScore);
}

void showGameOverInvaders() {
  if (score > highScore) {
    highScore = score;
  }

  tft.fillScreen(ST77XX_BLACK);
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
      initInvadersGame();
      invadersState = PLAYING_INVADERS;
      break;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      invadersInitialized = false;
      state = STATE_MENU;
      drawMenu();
      delay(200);
      break;
    }
  }
}

void invadersGame() {
  if (!invadersInitialized) {
    initInvadersGame();
    invadersInitialized = true;
  }

  if (invadersState == MENU_INVADERS) {
    showMenuInvaders();
  }
  else if (invadersState == PLAYING_INVADERS) {
    tft.fillScreen(ST77XX_BLACK);

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
  }
  else if (invadersState == GAMEOVER_INVADERS) {
    showGameOverInvaders();
  }
}