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

const int size = 10;
const int step = 10;

const char nivel[13][16] = {
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

int playerX, playerY;
int box1X, box1Y;
int box2X, box2Y;
int goal1X, goal1Y;
int goal2X, goal2Y;
bool victory = false;

void cargarEntidades() {
  int cajasEncontradas = 0;
  int metasEncontradas = 0;
  for (int y = 0; y < 13; y++) {
    for (int x = 0; x < 16; x++) {
      char ch = nivel[y][x];
      if (ch == 'L') { playerX = x * size; playerY = y * size; }
      if (ch == 'C') {
        if (cajasEncontradas == 0) { box1X = x * size; box1Y = y * size; cajasEncontradas++; }
        else { box2X = x * size; box2Y = y * size; }
      }
      if (ch == 'M') {
        if (metasEncontradas == 0) { goal1X = x * size; goal1Y = y * size; metasEncontradas++; }
        else { goal2X = x * size; goal2Y = y * size; }
      }
    }
  }
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

  tft.fillScreen(ST77XX_BLACK);
  cargarEntidades();

  for (int y = 0; y < 13; y++) {
    for (int x = 0; x < 16; x++) {
      int px = x * size;
      int py = y * size;
      char ch = nivel[y][x];
      if (ch == '1') tft.fillRect(px, py, size, size, ST77XX_WHITE);
      else if (ch == 'M') tft.fillRect(px, py, size, size, ST77XX_GREEN);
      else if (ch == '2') tft.fillRect(px, py, size, size, ST77XX_BLACK);
    }
  }

  drawScene();
}

void loop() {
  if (victory) return;
  int dx = 0, dy = 0;
  if (digitalRead(BTN_UP) == LOW) dy = -step;
  if (digitalRead(BTN_DOWN) == LOW) dy = step;
  if (digitalRead(BTN_LEFT) == LOW) dx = -step;
  if (digitalRead(BTN_RIGHT) == LOW) dx = step;

  if (digitalRead(BTN_START) == LOW) {
    victory = false;
    setup();
    delay(300);
    return;
  }

  if (dx != 0 || dy != 0) {
    int newPlayerX = playerX + dx;
    int newPlayerY = playerY + dy;

    if (!isInside(newPlayerX, newPlayerY) || isWall(newPlayerX, newPlayerY)) return;

    bool empujaCaja1 = isColliding(newPlayerX, newPlayerY, box1X, box1Y);
    bool empujaCaja2 = isColliding(newPlayerX, newPlayerY, box2X, box2Y);

    if (empujaCaja1 || empujaCaja2) {
      int& bX = empujaCaja1 ? box1X : box2X;
      int& bY = empujaCaja1 ? box1Y : box2Y;
      int newBoxX = bX + dx;
      int newBoxY = bY + dy;
      if (isInside(newBoxX, newBoxY) && !isWall(newBoxX, newBoxY) &&
          !isColliding(newBoxX, newBoxY, empujaCaja1 ? box2X : box1X, empujaCaja1 ? box2Y : box1Y)) {
        clearSprite(bX, bY);
        bX = newBoxX;
        bY = newBoxY;
        clearSprite(playerX, playerY);
        playerX = newPlayerX;
        playerY = newPlayerY;
        drawScene();
      }
    } else {
      clearSprite(playerX, playerY);
      playerX = newPlayerX;
      playerY = newPlayerY;
      drawScene();
    }

    if (((box1X == goal1X && box1Y == goal1Y) && (box2X == goal2X && box2Y == goal2Y)) ||
        ((box1X == goal2X && box1Y == goal2Y) && (box2X == goal1X && box2Y == goal1Y))) {
      tft.setTextColor(ST77XX_WHITE);
      tft.setTextSize(2);
      tft.setCursor(20, 80);
      tft.println("VICTORIA!");
      victory = true;
    }

    delay(100);
  }
}

void drawScene() {
  bool cajaEnMeta1 = (box1X == goal1X && box1Y == goal1Y) || (box2X == goal1X && box2Y == goal1Y);
  bool cajaEnMeta2 = (box1X == goal2X && box1Y == goal2Y) || (box2X == goal2X && box2Y == goal2Y);
  tft.fillRect(goal1X, goal1Y, size, size, cajaEnMeta1 ? ST77XX_BLUE : ST77XX_GREEN);
  tft.fillRect(goal2X, goal2Y, size, size, cajaEnMeta2 ? ST77XX_BLUE : ST77XX_GREEN);

  drawBox(box1X, box1Y);
  drawBox(box2X, box2Y);
  drawLarry(playerX, playerY);
}

void drawLarry(int x, int y) {
  tft.fillRect(x, y, size, size, ST77XX_RED);
}

void drawBox(int x, int y) {
  tft.fillRect(x, y, size, size, ST77XX_YELLOW);
}

void clearSprite(int x, int y) {
  int cx = x / size;
  int cy = y / size;
  char ch = nivel[cy][cx];
  if (ch == '1') tft.fillRect(x, y, size, size, ST77XX_WHITE);
  else if (ch == 'M') tft.fillRect(x, y, size, size, ST77XX_GREEN);
  else tft.fillRect(x, y, size, size, ST77XX_BLACK);
}

bool isInside(int x, int y) {
  return x >= 0 && x <= tft.width() - size && y >= 0 && y <= tft.height() - size;
}

bool isColliding(int ax, int ay, int bx, int by) {
  int margin = 4;
  return abs(ax - bx) < size - margin && abs(ay - by) < size - margin;
}

bool isWall(int x, int y) {
  int cx = x / size;
  int cy = y / size;
  return nivel[cy][cx] == '1';
}