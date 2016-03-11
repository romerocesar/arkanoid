#ifndef _bola_h
#define _bola_h

#include <GL/glut.h>

#define BOLA_RADIO 4
#define BOLA_DIAMETRO (BOLA_RADIO*2)
#define DEFAULT_BALL_SPEED_X 2
#define DEFAULT_BALL_SPEED_Y -5

typedef struct{
  int x;
  int y;

  int r;

  float velx;
  float vely;

  int paddle; 

} bola_t;

#include "util.h"
#include "paddle.h"

bola_t *initBola();
void destruirBola(bola_t *b);

void dibujarBola(bola_t *b);

int actualizarBola(bola_t *b);
void soltarBola(bola_t *b);

#endif
