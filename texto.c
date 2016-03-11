#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <GL/glut.h>
#include "util.h"

// TODO: el glPrint de nehe es mas avanzado
static GLuint base10;
static GLuint base24;

extern state_t state;

void initTexto(){
  int i;
  base10 = glGenLists(256);
  base24 = glGenLists(256);
  for (i = 0; i< 256; ++i){
    glNewList(base10 + i, GL_COMPILE);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, i);
    glEndList();

    glNewList(base24 + i, GL_COMPILE);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);
    glEndList();
  }
}


void glPrint10(int x, int y, const char *fmt, ...){
  char text[256];
  va_list ap;

  if (fmt == NULL)
    return;

  va_start(ap, fmt);
  vsprintf(text, fmt, ap);
  va_end(ap);
  
  glListBase(base10);
  glRasterPos2i(x,y);
  glCallLists(strlen(text), GL_BYTE, text);
}

void glPrint24(int x, int y, const char *fmt, ...){
  char text[256];
  va_list ap;

  if (fmt == NULL)
    return;

  va_start(ap, fmt);
  vsprintf(text, fmt, ap);
  va_end(ap);
  
  glListBase(base24);
  glRasterPos2i(x,y);
  glCallLists(strlen(text), GL_BYTE, text);
}

void GameText()
{
  int x=0,y=0;
  glLoadIdentity();

  //Help
  if(state.help){
    glColor3f(1.0,1.0,0.0);
    glPrint24(x=200,y=180,"HELP\n");
    glPrint24(x=100,y+=50,"Game\n");    
    glColor3f(1.0,1.0,1.0);
    glPrint10(x,y+=20,"H,h\tToggle this help message\n");
    glPrint10(x,y+=20,"P,p\tPause game\n");
    glPrint10(x,y+=20,"q\tQuit game\n");
    glPrint10(x,y+=20,"S,s\tIncrease,Decrease paddle speed\n");
    glColor3f(1.0,1.0,0.0);
    glPrint24(x=300,y=210,"Developer");
    glColor3f(1.0,1.0,1.0);
    glPrint10(x,y+=20,"A,a  Toggle Ambient light\n");
    glPrint10(x,y+=20,"C,c  Toggle Specular light\n");
    glPrint10(x,y+=20,"D,d  Toggle Diffuse light\n");
    glPrint10(x,y+=20,"L,l  Increase,Decrease light attenuation\n");
    glPrint10(x,y+=20,"T,t  Toggle textures\n");
    glPrint10(x,y+=20,"V,v  Verbose mode\n");
    glPrint10(x,y+=20,"W,w  Toggle Wireframe mode\n");
  }else if(state.pausado){
    x=200;y=300;
    glColor3f(1.0,0.0,0.0);
    glPrint24(x,y,"Pause\n");
  }

  x=490;y=0;
  glColor3f(0.0,1.0,0.0);
  glPrint24(x,y+=20,"STATUS\n");
  glColor3f(1.0,1.0,1.0);
  glPrint10(x,y+=20,"Level: %d\n",state.level);
  glPrint10(x,y+=20,"Lives: %d\n",state.lives);
  glPrint10(x,y+=20,"Score: %d\n",state.score);
  glPrint10(x,y+=20,"Bricks: %d\n",state.left_bricks);

  // Extras
  glColor3f(0.0,1.0,0.0);
  glPrint24(x,y+=30,"EXTRAS\n");
  glColor3f(1.0,1.0,1.0);
  glPrint10(x,y+=20,"Glue: %d\n",state.paddle.glue);
  glPrint10(x,y+=20,"Visible: %d\n",state.paddle.visible);
  glPrint10(x,y+=20,"Freeze: %d\n",state.paddle.freeze);

  // Verbose mode
  if(!state.VERBOSE) return;
  glColor3f(0.0,0.0,1.0);
  glPrint24(x,y+=70,"VERBOSE\n");
  glColor3f(1.0,1.0,1.0);
  glPrint10(x,y+=20,"Ball: %d,%d    Radio: %d\n",state.bola->x,state.bola->y,state.bola->r);
  glPrint10(x,y+=20,"Speed (x,y): %.2f,%.2f\n",state.bola->velx,state.bola->vely);
  glPrint10(x,y+=20,"mag(Speed): %.2f\n",hypot(state.bola->velx, state.bola->vely));
  glPrint10(x,y+=20,"Paddle:%3d    Speed:%2d\n",state.paddle.x,state.paddle.speed);
  glPrint10(x,y+=20,"Textures: %s\n",(state.show_textures)?"on":"off");
  glPrint10(x,y+=20,"Wireframe: %s\n",(state.wireframe)?"on":"off");
  glPrint10(x,y+=20,"Light att.: %.2f",state.light);
  //tipos de luces
  glPrint10(x,y+=20,"Ambient: %s",(state.la)?"on":"off");
  glPrint10(x,y+=20,"Diffuse: %s",(state.ld)?"on":"off");
  glPrint10(x,y+=20,"Specular: %s",(state.ls)?"on":"off");
}
