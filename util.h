#ifndef _util_h
#define _util_h
#include "paddle.h"
#include "bola.h"
#include "extra.h"
#include "brick.h"
#include "extra.h"

#define UPDATE_MS 10
#define BORDER 480

typedef struct{
    // objetos
    brick * bricks;
    bola_t * bola;
    extra_t * extras;
    paddle_t paddle;

    //variables globales
    int help; //help message
    int show_textures;
    int pausado;
    int over; //game over
    int lives;
    int level;
    int score;
    int left_bricks;
    int total_bricks;
    int VERBOSE;
    int wireframe;
    GLfloat light;

    //texturas
    GLuint brick_texture[1];
    GLuint extra_texture[NUM_EXTRAS];

  //luces
  int la;
  int ls;
  int ld;

}state_t;

typedef struct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat emission[4];
  GLfloat shininess[1];
}material;

void setMaterial(material *m);

#endif
