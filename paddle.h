#ifndef _paddle_h
#define _paddle_h
#include <GL/glut.h>

#define PADDLE_H 10
#define PADDLE_R PADDLE_H/2 //radio para dibujar el cilindro

typedef struct{
  int x; //esquina superior izquierda
  int y;
  int h; //altura
  int w; // anchura
  GLUquadricObj * qobj; //para glucylinder
  int speed;
  int visible;
  int glue;
  int freeze;
}paddle_t;

#include "util.h"

void InitPaddle();

void DisplayPaddle();

#endif
