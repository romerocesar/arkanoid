#ifndef _brick_h
#define _brick_h
#include <GL/glut.h>
#include <SDL/SDL.h>

#define BRICK_W 30
#define BRICK_H 15

typedef struct{
  int x;
  int y;
  int live;
  int extra;
} brick;

#include "bola.h"
#include "util.h"

void DisplayBrick(brick * b);

int BrickHit(brick *b, bola_t *bola);

int brickTextures();

void initBricks();

#endif
