#ifndef MAIN_H
#define MAIN_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "constantes.h"
#include "Larry.h"
#include "Simon.h"
#include "Invaders.h"


extern SPIClass mySPI;
extern Adafruit_ST7735 tft;

extern int estadoActual;
extern unsigned long lastDebounceTime;

void inicializarSistema();
void ejecutarLoop();
void drawMenu();
void menuLoop();

#endif
