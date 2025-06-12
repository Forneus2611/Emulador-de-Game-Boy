#include "main.h"

static const uint32_t larrySprite[100] = {
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
};

static const uint32_t cajaSprite[100] = {
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
};

static int larryX, larryY;
static int boxX[LARRY_MAX_CAJAS], boxY[LARRY_MAX_CAJAS];
static int goalX[LARRY_MAX_CAJAS], goalY[LARRY_MAX_CAJAS];
static int cajasEncontradas = 0;
static int metasEncontradas = 0;
static bool victory = false;
static bool pantallaInicioMostrada = false;
static unsigned long lastLarryDebounce = 0;
static int nivelActual = 0;

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

static const char (*niveles[])[LARRY_COLS] = {nivel1, nivel2, nivel3, nivel4, nivel5};
static const int TOTAL_NIVELES = sizeof(niveles) / sizeof(niveles[0]);
static const char (*nivel)[LARRY_COLS];

static void pantallaInicioLarry() {
  tft.fillScreen(COLOR_FONDO);
  tft.setTextColor(COLOR_LARRY);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.println("Larry's Adventures");
  tft.setTextSize(1);
  tft.setCursor(20, 80);
  tft.println("Presiona START");
}

static void pantallaNivelLarry(int numeroNivel) {
  tft.fillScreen(COLOR_FONDO);
  tft.setTextColor(COLOR_LARRY);
  tft.setTextSize(2);
  tft.setCursor(40, 40);
  tft.print("Nivel ");
  tft.println(numeroNivel);
  delay(1500);
}

static void drawLarry(int x, int y) {
  for (int i = 0; i < LARRY_SIZE; i++) {
    for (int j = 0; j < LARRY_SIZE; j++) {
      uint32_t color32 = larrySprite[i * LARRY_SIZE + j];
      if ((color32 & 0xFFFFFF) != 0x000000) {
        uint16_t color16 = tft.color565((color32 >> 16) & 0xFF, (color32 >> 8) & 0xFF, color32 & 0xFF);
        tft.drawPixel(x + j, y + i, color16);
      }
    }
  }
}
static void drawBox(int x, int y) {
  for (int i = 0; i < LARRY_SIZE; i++) {
    for (int j = 0; j < LARRY_SIZE; j++) {
      uint32_t color32 = cajaSprite[i * LARRY_SIZE + j];
      if ((color32 & 0xFFFFFF) != 0x000000) {
        uint16_t color16 = tft.color565((color32 >> 16) & 0xFF, (color32 >> 8) & 0xFF, color32 & 0xFF);
        tft.drawPixel(x + j, y + i, color16);
      }
    }
  }
}

static void clearSprite(int x, int y) {
  int cx = x / LARRY_SIZE;
  int cy = y / LARRY_SIZE;
  char ch = nivel[cy][cx];
  if (ch == '1') tft.fillRect(x, y, LARRY_SIZE, LARRY_SIZE, COLOR_WALL);
  else if (ch == 'M') tft.fillRect(x, y, LARRY_SIZE, LARRY_SIZE, COLOR_META);
  else tft.fillRect(x, y, LARRY_SIZE, LARRY_SIZE, COLOR_FONDO);
}

static bool isWall(int x, int y) {
  int cx = x / LARRY_SIZE;
  int cy = y / LARRY_SIZE;
  return nivel[cy][cx] == '1';
}

static bool isInside(int x, int y) {
  return x >= 0 && x <= tft.width() - LARRY_SIZE && y >= 0 && y <= tft.height() - LARRY_SIZE;
}

static bool isColliding(int ax, int ay, int bx, int by) {
  int margin = 4;
  return abs(ax - bx) < LARRY_SIZE - margin && abs(ay - by) < LARRY_SIZE - margin;
}

static void drawScene() {
  tft.fillScreen(COLOR_FONDO);
  for (int y = 0; y < LARRY_ROWS; y++) {
    for (int x = 0; x < LARRY_COLS; x++) {
      int px = x * LARRY_SIZE;
      int py = y * LARRY_SIZE;
      char ch = nivel[y][x];
      if (ch == '1') tft.fillRect(px, py, LARRY_SIZE, LARRY_SIZE, COLOR_WALL);
      else if (ch == 'M') tft.fillRect(px, py, LARRY_SIZE, LARRY_SIZE, COLOR_META);
    }
  }
  for (int i = 0; i < cajasEncontradas; i++) {
    drawBox(boxX[i], boxY[i]);
  }
  drawLarry(larryX, larryY);
}

static void setupNivel() {
  cajasEncontradas = 0;
  metasEncontradas = 0;

  for (int y = 0; y < LARRY_ROWS; y++) {
    for (int x = 0; x < LARRY_COLS; x++) {
      int px = x * LARRY_SIZE;
      int py = y * LARRY_SIZE;
      char ch = nivel[y][x];
      if (ch == 'M') {
        goalX[metasEncontradas] = px;
        goalY[metasEncontradas] = py;
        metasEncontradas++;
      } else if (ch == 'L') {
        larryX = px; larryY = py;
      } else if (ch == 'C') {
        boxX[cajasEncontradas] = px;
        boxY[cajasEncontradas] = py;
        cajasEncontradas++;
      }
    }
  }
  drawScene();
}

void larryGame() {
  static bool larryInitialized = false;
  if (!larryInitialized) {
    pantallaInicioMostrada = false;
    victory = false;
    nivelActual = 0;
    nivel = niveles[nivelActual];
    pantallaInicioLarry();
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
      estadoActual = STATE_MENU;
      drawMenu();
      delay(300);
    }
    return;
  }

  int dx = 0, dy = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastLarryDebounce > DEBOUNCE_LARRY) {
    if (digitalRead(BTN_UP) == LOW) { dy = -LARRY_STEP; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_DOWN) == LOW) { dy = LARRY_STEP; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_LEFT) == LOW) { dx = -LARRY_STEP; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_RIGHT) == LOW) { dx = LARRY_STEP; lastLarryDebounce = currentTime; }
    else if (digitalRead(BTN_START) == LOW) {
      setupNivel();
      lastLarryDebounce = currentTime;
      return;
    }
  }

  if (dx != 0 || dy != 0) {
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
      tft.setTextColor(COLOR_LARRY);
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
        tft.fillScreen(COLOR_FONDO);
        tft.setCursor(10, 40);
        tft.setTextSize(2);
        tft.setTextColor(COLOR_META);
        tft.println("JUEGO COMPLETADO!");
        victory = true;
      }
    }
  }

  if (digitalRead(BTN_BACK) == LOW) {
    larryInitialized = false;
    estadoActual = STATE_MENU;
    drawMenu();
    delay(200);
  }
}

