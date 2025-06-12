#ifndef SIMON_H
#define SIMON_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"  

#define SIMON_MAX_SEQ 32    

void simonGame(void);      

static void showStartScreenSimon(void);
static void showSequenceSimon(void);
static void showColorSimon(int dir);
static void gameOverScreenSimon(void);

static int waitForButtonSimon(void);
static bool playerTurnSimon(void);

#endif
