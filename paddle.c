#include <stdio.h>
#include "paddle.h"

extern state_t state;
// TODO: poner un material metalico
static material materialPaddle = {
  {0.2, 0.2, 0.2, 1.0}, //ambient
  {0.0, 0.0, 0.0, 1.0},//diffuse
  {1.0, 1.0, 1.0, 1.0},//specular
  {0.5, 0.5, 0.5, 1.0},//emission
  {10.0}
};

void InitPaddle()
{
  state.paddle.x=225;
  state.paddle.y=450;
  state.paddle.w=50;
  state.paddle.h=PADDLE_H;
  state.paddle.qobj=gluNewQuadric();
  state.paddle.speed=5;
  state.paddle.freeze=0;
  state.paddle.visible=1;
}

void DisplayPaddle()
{
  if(state.over)
    return;
  if(state.paddle.visible){

    // cilindro
    if(state.wireframe){
      glLoadIdentity();
      glColor3f(0.4,0.4,0.4);
      glBegin(GL_LINE_LOOP);
      glVertex2f(state.paddle.x,state.paddle.y);
      glVertex2f(state.paddle.x+state.paddle.w,state.paddle.y);
      glVertex2f(state.paddle.x+state.paddle.w,state.paddle.y+PADDLE_H);
      glVertex2f(state.paddle.x,state.paddle.y+PADDLE_H);
      glEnd();
    }else{
      glPushMatrix();
      glEnable(GL_LIGHTING);
      glLoadIdentity();
      glTranslatef(state.paddle.x,state.paddle.y+PADDLE_R,0.0);
      glRotatef(90,0,1,0);
      setMaterial(&materialPaddle);
      gluCylinder(state.paddle.qobj,PADDLE_R,PADDLE_R,state.paddle.w,100,50);
      glDisable(GL_LIGHTING);
      glPopMatrix();
    }
  }
}

