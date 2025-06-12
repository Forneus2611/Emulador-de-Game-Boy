#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

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
#define BTN_START  7
#define BTN_BACK   6

const int size = 10;
const int step = 10;
const int MAX_CAJAS = 5;

int playerX, playerY;
int boxX[MAX_CAJAS], boxY[MAX_CAJAS];
int goalX[MAX_CAJAS], goalY[MAX_CAJAS];
int cajasEncontradas = 0;
int metasEncontradas = 0;
bool victory = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150;

// Sprites de Larry y Caja
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
// Niveles
const char nivel1[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
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
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel2[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
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
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel3[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
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
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel4[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
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
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

const char nivel5[13][16] = {
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
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
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};


// Array de niveles
const char (*niveles[])[16] = {nivel1, nivel2, nivel3, nivel4, nivel5};
const int TOTAL_NIVELES = sizeof(niveles) / sizeof(niveles[0]);
int nivelActual = 0;
const char (*nivel)[16] = niveles[nivelActual];

// Funciones
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
  drawLarry(playerX, playerY);
}

void pantallaNivel(int numeroNivel) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 40);
  tft.print("Nivel ");
  tft.println(numeroNivel);
  delay(1500);
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
        playerX = px; playerY = py;
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

void setup() {
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);

  pantallaNivel(nivelActual + 1);
  setupNivel();
}

void loop() {
  if (victory) return;

  int dx = 0, dy = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastDebounceTime > debounceDelay) {
    if (digitalRead(BTN_UP) == LOW) { dy = -step; lastDebounceTime = currentTime; }
    else if (digitalRead(BTN_DOWN) == LOW) { dy = step; lastDebounceTime = currentTime; }
    else if (digitalRead(BTN_LEFT) == LOW) { dx = -step; lastDebounceTime = currentTime; }
    else if (digitalRead(BTN_RIGHT) == LOW) { dx = step; lastDebounceTime = currentTime; }
    else if (digitalRead(BTN_START) == LOW) {
      victory = false;
      setupNivel();
      lastDebounceTime = currentTime;
      return;
    }
  }

  if (dx != 0 || dy != 0) {
    int newPlayerX = playerX + dx;
    int newPlayerY = playerY + dy;

    if (!isInside(newPlayerX, newPlayerY) || isWall(newPlayerX, newPlayerY)) return;

    bool empuja = false;
    for (int i = 0; i < cajasEncontradas; i++) {
      if (isColliding(newPlayerX, newPlayerY, boxX[i], boxY[i])) {
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
          clearSprite(playerX, playerY);
          playerX = newPlayerX;
          playerY = newPlayerY;
          drawScene();
          empuja = true;
          break;
        } else {
          return; // No puede empujar
        }
      }
    }

    if (!empuja) {
      clearSprite(playerX, playerY);
      playerX = newPlayerX;
      playerY = newPlayerY;
      drawScene();
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
        victory = false;
        pantallaNivel(nivelActual + 1);
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
}
