#ifndef LARRY_H
#define LARRY_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h" 



extern const uint32_t larrySprite[100];
extern const uint32_t cajaSprite[100];



extern int larryX, larryY;  
extern int boxX[LARRY_MAX_CAJAS], boxY[LARRY_MAX_CAJAS];  
extern int goalX[LARRY_MAX_CAJAS], goalY[LARRY_MAX_CAJAS];  
extern int cajasEncontradas;  
extern int metasEncontradas;  

extern bool victory;
extern bool pantallaInicioMostrada;
extern unsigned long lastLarryDebounce;
extern int nivelActual;


extern const char nivel1[13][16];
extern const char nivel2[13][16];
extern const char nivel3[13][16];
extern const char nivel4[13][16];
extern const char nivel5[13][16];

extern const char (*niveles[])[LARRY_COLS];
extern const int TOTAL_NIVELES;
extern const char (*nivel)[LARRY_COLS];  


void drawLarry(int x, int y);
void drawBox(int x, int y);
void drawGoal(int x, int y);
void drawWall(int x, int y);
void drawFloor(int x, int y);
void drawVictoryScreen(void);


void pantallaInicioLarry(void);
void pantallaNivelLarry(int numeroNivel);
void cargarNivel(int numeroNivel);
void resetearJuego(void);
void actualizarJuego(void);
void verificarVictoria(void);
void manejarMovimiento(int dx, int dy);
bool hayCajaEn(int x, int y);
bool hayMetaEn(int x, int y);
bool hayParedEn(int x, int y);



#endif 
