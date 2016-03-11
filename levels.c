#include "levels.h"

extern state_t state;
//TODO:crear los otros 4 niveles. ahorita cuando pasa el nivel vuelve a cargar el primero
int level1()
{
  state.total_bricks=90;
  state.left_bricks=state.total_bricks;
  if(state.bricks!=NULL)
    free(state.bricks);
  state.bricks=(brick *)calloc(90,sizeof(brick));
  if(state.bricks==NULL) return 1;
  if(state.extras!=NULL)
    free(state.extras);
  state.extras=(extra_t *)calloc(90,sizeof(extra_t));
  if(state.extras==NULL) return 1;

  int i;
  for(i=0;i<15*6;i++){
    brick *b = &state.bricks[i];
    b->x = i%15;
    b->y = i/15;
    if( (6<=b->x && b->x<=7) || 4<b->y)
      b->live=5;
    else
      b->live=1;
    state.extras[i].extra=0; //no hay extras cayendo
    float r=random()/(RAND_MAX*1.0);
    //todos los extras posibles
    if(r<0.05)
      b->extra=EXTRA_GLUE;
    else if(r<0.10)
      b->extra=EXTRA_LARGER;
    else if(r<0.15)
      b->extra=EXTRA_LIVE;
    else if(r<0.20)
      b->extra=EXTRA_SMALLER;
    else if(r<0.25)
      b->extra=EXTRA_INVISIBLE;
    else if(r<0.30)
      b->extra=EXTRA_SCORE_200;
    else if(r<0.35)
      b->extra=EXTRA_SCORE_500;
    else if(r<0.40)
      b->extra=EXTRA_SCORE_1000;
    else if(r<0.45)
      b->extra=EXTRA_SCORE_5000;
    else if(r<0.50)
      b->extra=EXTRA_SCORE_2000;
    else if(r<0.55)
      b->extra=EXTRA_SCORE_10000;
    else if(r<0.60)
      b->extra=EXTRA_FREEZE;
    else if(r<0.65)
      b->extra=EXTRA_FAST;
    else if(r<0.70)
      b->extra=EXTRA_SLOW;
    else
      b->extra=0;
  }
  return 0;
}

int level2()
{
  state.total_bricks=90;
  state.left_bricks=state.total_bricks;
  if(state.bricks!=NULL)
    free(state.bricks);
  state.bricks=(brick *)calloc(90,sizeof(brick));
  if(state.bricks==NULL) return 1;
  if(state.extras!=NULL)
    free(state.extras);
  state.extras=(extra_t *)calloc(90,sizeof(extra_t));
  if(state.extras==NULL) return 1;

  int i;
  for(i=0;i<15*6;i++){
    brick *b = &state.bricks[i];
    b->x = i%15;
    b->y = i/15;
    if(b->y==0 || //primera fila
       (b->y==1 && 11<=b->x && b->x<=13)|| //segunda
       (b->y==2 && 8<=b->x && b->x<=10)|| //tercera
       (b->y==3 && 5<=b->x && b->x<=7)|| //cuarta
       (b->y==4 && 1<=b->x && b->x<=4)|| //quinta
       b->y==5  //ultima fila
       )
      b->live=5;
    else
      b->live=1;
    state.extras[i].extra=0; //no hay extras cayendo
    float r=random()/(RAND_MAX*1.0);
    if(r<0.05)
      b->extra=EXTRA_LARGER;
    else if(r<0.15)
      b->extra=EXTRA_LIVE;
    else if(r<0.20)
      b->extra=EXTRA_INVISIBLE;
    else if(r<0.30)
      b->extra=EXTRA_SCORE_200;
    else if(r<0.35)
      b->extra=EXTRA_SCORE_500;
    else if(r<0.40)
      b->extra=EXTRA_SCORE_1000;
    else if(r<0.45)
      b->extra=EXTRA_SCORE_5000;
    else if(r<0.50)
      b->extra=EXTRA_SCORE_2000;
    else if(r<0.55)
      b->extra=EXTRA_SCORE_10000;
    else if(r<0.60)
      b->extra=EXTRA_FREEZE;
    else
      b->extra=0;
  }
  return 0;
}

void DisplayLevel()
{
  if(state.over){
    glPrint24(180,200,"GAME OVER\n");
    return; 
  }
  int i;
  for(i=0;i<state.total_bricks;i++){
    if (state.bricks[i].live){
      DisplayBrick(&state.bricks[i]);
    }
  }
}

int loadLevel(int lvl)
{
  // estado
  state.over=0;
  state.pausado=0;
  state.lives=3;
  state.score=0;
  state.show_textures=1;
  state.help=0;
  state.light=1.0;
  state.wireframe=0;
  state.bola=initBola();

  cleanExtras();

  int st=0;
  switch(lvl){
  case 1:
    st=level1();
    break;
  case 2:
    st=level2();
    break;
  case 3:
  case 4:
  case 5:
  default:
    st=level1();
    break;
  }

  return st;
}
