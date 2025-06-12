#ifndef CONSTANTES_H
#define CONSTANTES_H

// Configuración TFT
#define TFT_CS    10
#define TFT_DC    8
#define TFT_RST   9
#define TFT_SCLK  12
#define TFT_MOSI  11

#define TFT_ROTATION   1

// Botones
#define BTN_UP     2
#define BTN_DOWN   3
#define BTN_LEFT   4
#define BTN_RIGHT  5
#define BTN_BACK   6
#define BTN_START  7

// Estados de sistema
#define STATE_MENU      0
#define STATE_LARRY     1
#define STATE_SIMON     2
#define STATE_INVADERS  3

// Juego Larry
#define LARRY_SIZE        10
#define LARRY_STEP        10
#define LARRY_MAX_CAJAS   5
#define LARRY_TOTAL_NIVELES 5
#define LARRY_ROWS        13
#define LARRY_COLS        16

// Juego Simon
#define SIMON_MAX_SEQ     100
#define SIMON_COLOR_UP    ST77XX_BLUE
#define SIMON_COLOR_RIGHT ST77XX_YELLOW
#define SIMON_COLOR_DOWN  ST77XX_GREEN
#define SIMON_COLOR_LEFT  ST77XX_RED

// Juego Invaders
#define SCREEN_WIDTH     128
#define SCREEN_HEIGHT    160
#define INVADER_SHOTS    5
#define INVADER_ENEMIES  6

// Antirrebote
#define DEBOUNCE_DELAY   200
#define DEBOUNCE_LARRY   150

// Colores
#define COLOR_WALL   tft.color565(100, 100, 100)
#define COLOR_META   ST77XX_GREEN
#define COLOR_FONDO  ST77XX_BLACK
#define COLOR_LARRY  ST77XX_WHITE

#endif
