#ifndef INVADERS_H
#define INVADERS_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"  


void invadersGame(void);          

static void initEnemiesInvaders(void);
static void drawPlayerInvaders(void);
static void drawEnemyInvaders(int x, int y);
static void drawEnemiesInvaders(void);
static void drawShotsInvaders(void);
static void drawScoreInvaders(void);


static void moveEnemiesInvaders(void);
static void moveShotsInvaders(void);
static void readButtonsInvaders(void);

static void showMenuInvaders(void);
static void showGameOverInvaders(void);


static bool allEnemiesDeadInvaders(void);
static void nextWaveInvaders(void);

#endif 
