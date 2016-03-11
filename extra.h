#ifndef _bonus_h
#define _bonus_h
#include <GL/glut.h>
#include <stdio.h>
#include <SDL/SDL.h>

#define SCORE_BRICK 100

enum {EXTRA_NONE=0,
      EXTRA_LIVE,
      EXTRA_SMALLER,
      EXTRA_LARGER,
      EXTRA_GLUE,
      EXTRA_FAST,
      EXTRA_SLOW,
      EXTRA_INVISIBLE,
      EXTRA_FREEZE,
      EXTRA_BALL,
      
      EXTRA_SCORE_200,
      EXTRA_SCORE_500,
      EXTRA_SCORE_1000,
      EXTRA_SCORE_2000,
      EXTRA_SCORE_5000,
      EXTRA_SCORE_10000,

      // dejar este de ultimo
      NUM_EXTRAS
};

typedef struct {
  int x;
  int y;
  int extra;
} extra_t;

#include "brick.h"
#include "util.h"
 
void initExtras();
void displayExtra(extra_t * e);
void displayExtras();

// el paddle encuentra el extra
int extraHit(extra_t * e);

int updateExtras();

void applyExtra(int e);

void cleanExtras();

int extraTextures();
#endif
