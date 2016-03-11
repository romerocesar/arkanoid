#include "extra.h"

//TODO: 
//      desactivar el glue con el tiempo

extern state_t state;

static GLuint extralistid;

// muestra uno con su color determinado
void displayExtra(extra_t * e)
{
  glLoadIdentity();
  if(state.show_textures){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,state.extra_texture[e->extra]);
  }


  glTranslatef(e->x, e->y, 0);
  glCallList(extralistid);
  glDisable(GL_TEXTURE_2D);
}

void initExtras(){
    extralistid = glGenLists(1);
    glNewList(extralistid, GL_COMPILE);

    glColor3f(1.0f,1.0f,1.0f);
    //dibujo el extra con su textura
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0,1.0);glVertex3f(0,0,0.0);
    glTexCoord2f(1.0,1.0);glVertex3f(BRICK_W,0.0,0.0);
    glTexCoord2f(1.0,0.0);glVertex3f(BRICK_W,BRICK_H,0.0);
    glTexCoord2f(0.0,0.0);glVertex3f(0,BRICK_H,0.0);
    glEnd();

    glEndList();
    
}

// muestra todos
void displayExtras()
{
  int i;
  for(i=0;i<state.total_bricks;i++)
    if(state.extras[i].extra>0)
      displayExtra(&state.extras[i]);
}

// el paddle encuentra el extra
int extraHit(extra_t * e){
  if(e->x+BRICK_W>state.paddle.x && 
     e->x<state.paddle.x+state.paddle.w &&
     e->y+BRICK_H>state.paddle.y ){
    return 1;
  }
  return 0;
}

int updateExtras()
{
    int i;
    int cambio = 0;
    for(i=0;i<state.total_bricks;i++)
        if(state.extras[i].extra){
            cambio = 1;
            state.extras[i].y++;
            // pego con el paddle
            if(extraHit(&state.extras[i])){
                applyExtra(state.extras[i].extra);
                state.extras[i].extra=0;	 
            }
            // se sale de la pantalla
            if(state.extras[i].y>480)
                state.extras[i].extra=0;
        }

    return cambio;
}

/* Aplica el extra que recibe como parametro */
void applyExtra(int e)
{
  //deshabilito la invisibilidad, glue y freeze por haber agarrado
  //otro extra
  state.paddle.glue=0;
  state.paddle.visible=1;
  state.paddle.freeze=0;
  switch(e){

  case EXTRA_LIVE:
    state.lives++;
    break;

  case EXTRA_LARGER:
    state.paddle.w+=10;
    break;

  case EXTRA_SMALLER:
    if(state.paddle.w>=20)
      state.paddle.w-=10;
    break;

  case EXTRA_INVISIBLE:
    state.paddle.visible=0;
    break;

  case EXTRA_FREEZE:
    state.paddle.freeze=1;
    break;

  case EXTRA_GLUE:
    /*   TODO: hacer que funciones por un tiempo T, actualmente
     funciona por un solo rebote */
    state.paddle.glue=1;
    break;

  case EXTRA_SCORE_200:
      state.score += 200;
      break;

  case EXTRA_SCORE_500:
      state.score += 500;
      break;

  case EXTRA_SCORE_1000:
      state.score += 1000;
      break;

  case EXTRA_SCORE_2000:
      state.score += 2000;
      break;

  case EXTRA_SCORE_5000:
      state.score += 5000;
      break;

  case EXTRA_SCORE_10000:
      state.score += 10000;
      break;

  case EXTRA_FAST:
      // subir la velocidad 10%
      // TODO: poner un limite
      state.bola->velx *= 1.25;
      state.bola->vely *= 1.25;
      printf("faster\n");
      break;

  case EXTRA_SLOW:
      // bajar la velocidad 10%
      // TODO:poner un limite
      state.bola->velx *= 0.75;
      state.bola->vely *= 0.75;
      printf("slower\n");
      break;

  default:
    printf("Unknown extra:%d\n",e);
    break;
  }
}

/* Desaparece todos los extras cayendo */
void cleanExtras()
{
  int i;
  for(i=0;i<state.total_bricks;i++)
    state.extras[i].extra=0;
}

/* carga texturas en state.extra_texture */
int extraTextures()
{
  int st=1;
  //  state.extra_texture=(GLuint *)malloc(sizeof(GLuint)*NUM_EXTRAS);
  glGenTextures(NUM_EXTRAS,state.extra_texture);
  int i;
  char img[11]="data/xx.bmp";
  SDL_Surface *TextureImage; 
  for(i=1;i<=8;i++){
      img[6]=(i+48);//ascii
      if ((TextureImage=SDL_LoadBMP(img))){
          st = 0;
          glBindTexture( GL_TEXTURE_2D, state.extra_texture[i] );
          glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage->w,
                        TextureImage->h, 0, GL_BGR,
                        GL_UNSIGNED_BYTE, TextureImage->pixels );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
          glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
          glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
          glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
          SDL_FreeSurface(TextureImage);
      }
  }
  char img2[13]="data/xxx.bmp\0";
  for(i=10;i<=15;i++){// extra_score_xxx
    img2[6]=(i/10)+48;
    img2[7]=(i%10)+48;
    if((TextureImage=SDL_LoadBMP(img2))){
          st = 0;
          glBindTexture( GL_TEXTURE_2D, state.extra_texture[i] );
          glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage->w,
                        TextureImage->h, 0, GL_BGR,
                        GL_UNSIGNED_BYTE, TextureImage->pixels );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
          glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
          glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
          glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
          SDL_FreeSurface(TextureImage);
      }

  }
  return st;
}
