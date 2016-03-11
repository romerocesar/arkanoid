#include "brick.h"

extern state_t state;

static GLuint bricklistid;

void DisplayBrick(brick * b)
{
  if(!b->live)
    return;
  glLoadIdentity();
  // a mayor vida mas oscuridad
  glColor3f(1.0/b->live,1.0/b->live,1.0/b->live);

  int x = 8+b->x*31;
  int y = 50+b->y*16;

  //dibujo el brick con su textura
  if(state.show_textures){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,state.brick_texture[0]);
  }

  if(state.wireframe){
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);glVertex2f(x+BRICK_W,y);
    glVertex2f(x+BRICK_W,y+BRICK_H);glVertex2f(x,y+BRICK_H);
    glEnd();
  }else{
    glTranslatef(x,y, 0);
    glCallList(bricklistid);
  }
  glDisable(GL_TEXTURE_2D);

}

void initBricks(){
    bricklistid = glGenLists(1);
    glNewList(bricklistid, GL_COMPILE);

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0,1.0);glVertex3f(0,0,0.0);
    glTexCoord2f(1.0,1.0);glVertex3f(BRICK_W,0,0.0);
    glTexCoord2f(1.0,0.0);glVertex3f(BRICK_W,BRICK_H,0.0);
    glTexCoord2f(0.0,0.0);glVertex3f(0,BRICK_H,0.0);
    glEnd();

    glEndList();
}


/*
 * BrickHit: determina si bola golpeo a b.
 * Devuelve:
 *  0: si no hay hit
 *  1: si hay hit por arriba o por abajo
 *  -1: si hay hit por un lado
 */
int BrickHit(brick *b, bola_t *bola) {

    const int xc1 = bola->x - BOLA_RADIO;
    const int yc1 = bola->y - BOLA_RADIO;
    const int xc2 = bola->x + BOLA_DIAMETRO;
    const int yc2 = bola->y + BOLA_DIAMETRO;

    const int xcmed = bola->x;
    const int ymed = bola->y;

    const int xr1 = 8+b->x*31;
    const int yr1 = 50+b->y*16;
    const int xr2 = xr1 + 30;
    const int yr2 = yr1 + 15;

  

    if ( xr1 <= xcmed && xcmed <= xr2 && yc1 <= yr2 && yc1 > yr1)
        return 1;
    else if (xr1 <= xcmed && xcmed <= xr2 && yc2 > yr1 && yc2 < yr2)
        return 1;

    else if (yr1 <= ymed && ymed <= yr2 && xc2 >= xr1 && xc2 < xr2)
        return -1;
    else if (yr1 <= ymed && ymed <= yr2 && xc1 < xr2 && xc1 > xr1)
        return -1;

      

    return 0;
}

// carga la textura del brick.
int brickTextures()
{
  int st=1;
  glGenTextures(1,state.brick_texture);
  SDL_Surface *TextureImage;
  if ((TextureImage=SDL_LoadBMP("data/brick.bmp"))) {
      st = 0;
      glBindTexture( GL_TEXTURE_2D, state.brick_texture[0] );
      glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage->w,
                    TextureImage->h, 0, GL_BGR,
                    GL_UNSIGNED_BYTE, TextureImage->pixels );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      SDL_FreeSurface( TextureImage );
  }

  return st;
}
