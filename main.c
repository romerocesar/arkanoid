#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <GL/glut.h>
#include <SDL/SDL.h>
#include "util.h"
#include "paddle.h"
#include "texto.h"
#include "bola.h"
#include "levels.h"
#include "brick.h"

state_t state;


static int phi  = 45;
static int posmuneco = 0;
static int regresando = 0;
GLUquadricObj *cabezamuneco; //para glucylinder

static GLfloat lp[] = {BORDER/2.0, BORDER/2.0, -20.0, 1.0};
static GLfloat la[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat ld[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat ls[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat spot[] = {0.0, 0.0, -1.0};

/* carga todas las texturas en memoria */
int LoadGLTextures()
{
  int st=0;
  st|=brickTextures();
  st|=extraTextures();
  return st;
}

void muneco(){

    glColor3f(1,1,0);
    // brazo izquierdo
    glPushMatrix();
    {
        glTranslatef(-2, 2, 0);
        

        glPushMatrix();
        {
            glRotatef(-abs(phi), 0, 0, 1);
            glRectf(1,0, 0, -3);
        }
        glPopMatrix();
        
        glRotatef(-abs(phi), 0, 0, 1);
        glTranslatef(0.5, -3.2, 0);
        glColor3f(1, 102/255.0, 1);
        gluDisk(cabezamuneco, 0.3, 1.0, 20, 20);


    }
    glPopMatrix();

    // brazo derecho
    glColor3f(1,1,0);
    glPushMatrix();
    {
        glTranslatef(2, 2, 0);
        glRotatef(abs(phi), 0, 0, 1);
        glRectf(-1,0, 0, -3);
        
    }
    glPopMatrix();


    glColor3f(0, 0, 1);

    // pierna derecha
    glPushMatrix();
    {
        glTranslatef(2, -1, 0);
        glRotatef(abs(phi)-45, 0, 0, 1);
        glRectf(-1.5,0, 0, -3);
        
    }
    glPopMatrix();

    // pierna izquierda
    glPushMatrix();
    {
        glTranslatef(-2, -1, 0);
        glRotatef(-abs(phi)+45, 0, 0, 1);
        glRectf(1.5,0, 0, -3);
        
    }
    glPopMatrix();


    // el torso y la cabeza de ultimo para que queden por encima de lo demas
    glPushMatrix();
    {
        glColor3f(1, 1, 1);
        // el torso
        glRectf(-2,2, 2, -2);
        
        
        glColor3f(1, 1, 0);
        // la cabeza
        glTranslatef(0, 3, 0);
        if (phi % 2 == 0){
            glTranslatef(0, 0.5, 0);
        }
        //glRectf(-1,1, 1, -1);
        gluDisk(cabezamuneco, 0, 1, 20, 20);
    }
    glPopMatrix();



}

void setupLigths(){
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, lp);
  glLightfv(GL_LIGHT0, GL_AMBIENT, la);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
  glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot);
  glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 180); 
  glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,state.light);

  state.la=(la[0]==1.0)?1:0;
  state.ld=(ld[0]==1.0)?1:0;
  state.ls=(ls[0]==1.0)?1:0;

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);    
  glDisable(GL_LIGHTING);


  glLoadIdentity();

  glColor3f(1,1,1);
  glBegin(GL_LINE_LOOP);
  {
    glVertex2f(BORDER,BORDER);
    glVertex2f(0,BORDER);    glVertex2f(0,0.1);
    glVertex2f(BORDER,0);
  }
  glEnd();

  //level
  DisplayLevel();
  displayExtras();

  //paddle
  DisplayPaddle();
  //bola
  dibujarBola(state.bola);


  // figura articulada/muneco
  glLoadIdentity();
  glTranslatef(560+posmuneco, 215, 0);
  glRotatef(180, 0, 0, 1);
  glScalef(4, 4, 4);
  muneco();

  //text por encima de todo
  GameText();

  
  glutSwapBuffers();
}

void reshape(int ww, int hh)
{
  glViewport(0,0,ww,hh);				
	
  glMatrixMode(GL_PROJECTION);			
  glLoadIdentity();						

  glOrtho(0.0f,ww,hh,0.0f,-50.0f,50.0f);	

  glMatrixMode(GL_MODELVIEW);			
  glLoadIdentity();						
}


int init(){
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glutSetCursor(GLUT_CURSOR_NONE);

  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);

  cabezamuneco = gluNewQuadric();
  
  initBricks();
  initExtras();
  initTexto();
  InitPaddle();

  //level 
  state.level=1;
  loadLevel(state.level);

  setupLigths();

  if(LoadGLTextures())
    return 1;

  return 0;
}

void mouse(int button, int st, int x, int y){
  if (button == GLUT_LEFT_BUTTON){
    state.pausado=0;
    state.help=0;
    if(state.over){
      state.level=1;
      loadLevel(state.level);
      state.over=0;
    }
    else if (state.bola->paddle){
      state.paddle.glue=0;
      soltarBola(state.bola);
    }
    glutPostRedisplay();
  }
}

void motion(int x, int y)
{
    if(state.paddle.freeze || state.pausado || state.over)
        return;
    
    if(x<BORDER-state.paddle.w){
        if (x!=state.paddle.x){
            state.paddle.x=x;
            glutPostRedisplay();
        }
    }
}

void actualizar(int value){
    int cambio = 0;
    if(!state.pausado && !state.over){
        cambio |= actualizarBola(state.bola);
        cambio |= updateExtras();
    }
    if(!state.left_bricks){
        cambio = 1;
        loadLevel(++state.level);
    }
    if (cambio)
        glutPostRedisplay();
    glutTimerFunc(UPDATE_MS, actualizar, 10);
}

void actualizarMuneco(int value){    

    if (abs(posmuneco) > 45)
        regresando = !regresando;

    if (regresando)
        posmuneco -= 2;
    else
        posmuneco += 2;

    phi += 3;
    if (phi > 100){
        phi = -phi;
    } else if ( phi < 0 && phi > -40){
        phi = -phi;
    }

    glutTimerFunc(100, actualizarMuneco, 100);
    glutPostRedisplay();
}


void special(int key, int x, int y){
    switch(key){
    case GLUT_KEY_RIGHT:
        if (state.paddle.x + state.paddle.speed < BORDER-state.paddle.w)
            state.paddle.x+=state.paddle.speed;
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        if (state.paddle.x - state.paddle.speed > 0)
            state.paddle.x-=state.paddle.speed;
        glutPostRedisplay();
        break;
    default:
        printf("Key %d not binded\n",key);
        break;
    }
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
    case 'A'://ambient light
    case 'a':
      if(la[0]==1.0){
        la[0]=0.0;la[1]=0.0;la[2]=0.0;
      }else{
        la[0]=1.0;la[1]=1.0;la[2]=1.0;
      }
      setupLigths();
      break;
    case 'C'://specular light
    case 'c':
      if(ls[0]==1.0){
        ls[0]=0.0;ls[1]=0.0;ls[2]=0.0;
      }else{
        ls[0]=1.0;ls[1]=1.0;ls[2]=1.0;
      }
      setupLigths();
      break;
    case 'D'://diffuse light
    case 'd':
      if(ld[0]==1.0){
        ld[0]=0.0;ld[1]=0.0;ld[2]=0.0;
      }else{
        ld[0]=1.0;ld[1]=1.0;ld[2]=1.0;
      }
      setupLigths();
      break;
    case 'H':
    case 'h':
        state.help=(state.help+1)%2;
        state.pausado=1;
        glutPostRedisplay();
        break;
    case 'L':
        if(state.light<5.0)
            state.light+=0.1;
        setupLigths();
        glutPostRedisplay();
        break;
    case 'l':
        if(state.light>0.0)
            state.light-=0.1;
        setupLigths();
        glutPostRedisplay();
        break;
    case 'p':
    case 'P':
        state.pausado = !state.pausado;
        glutPostRedisplay();
        break;
    case 'q':
        exit(0);
        break;
    case 'R':
      state.bola->r++;
      break;
    case 'r':
      if(state.bola->r>4)
        state.bola->r--;
      break;
    case 's':
        if(state.paddle.speed>1)
            state.paddle.speed-=1;
        glutPostRedisplay();
        break;
    case 'S':
        if(state.paddle.speed<25)
            state.paddle.speed+=1;
        glutPostRedisplay();
        break;
    case 'T':
    case 't':
        state.show_textures=(state.show_textures+1)%2;
        glutPostRedisplay();
        break;
    case 'v':
    case 'V':
        state.VERBOSE=(state.VERBOSE+1)%2;
        glutPostRedisplay();
        break;
    case 'W':
    case 'w':
        state.wireframe=(state.wireframe+1)%2;
        break;
    default:
        break;
    }
}

int main(int argc, char **argv){

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  glutInitWindowPosition(50, 50);
  glutInitWindowSize(640, BORDER);
  glutCreateWindow("Arkanoid");

  if(init())
    return 1;

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutPassiveMotionFunc(motion);

  glutTimerFunc(UPDATE_MS, actualizar, 10);
  glutTimerFunc(100, actualizarMuneco, 100);

  glutMainLoop();

  return 0;
}
