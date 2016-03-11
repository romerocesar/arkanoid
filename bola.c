#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bola.h"
#include "util.h"

// esto supuestamente es rojo plastico
static material materialBola = {
  {0.5, 0.0, 0.0, 1.0},//ambient
  {1.0, 0.5, 0.5, 1.0},//difuse
  {0.0, 0.0, 0.0, 1.0},//specular
  {0.0, 0.0, 0.0, 1.0},//emission
  {32.0}
};

extern state_t state;

bola_t *initBola() {
    bola_t *b = malloc(sizeof(bola_t));
    b->x = 0;
    b->y = 0;
    b->r = BOLA_RADIO;
    b->velx = DEFAULT_BALL_SPEED_X;
    b->vely = DEFAULT_BALL_SPEED_Y;
    b->paddle = 1;

    return b;
}

void destruirBola(bola_t *b) {
    free(b);
}

void dibujarBola(bola_t *b){
    if(state.over)
        return;

    glLoadIdentity();
    glEnable(GL_LIGHTING);

    if (b->paddle){
        // la bola esta en el paddle
        glTranslatef(state.paddle.x + state.paddle.w/2, state.paddle.y - 5, 0);
    } else {
        // la bola esta en juego
        glTranslatef(b->x, b->y, 0);
    }
    glColor3f(1.0,0.0,0.0);
    setMaterial(&materialBola);
    if(state.wireframe)
      glutWireSphere(state.bola->r, 10, 10);
    else{
      glutSolidSphere(BOLA_RADIO, 25, 25);
    }
    glDisable(GL_LIGHTING);

}

void soltarBola(bola_t *b){
    b->x = state.paddle.x + state.paddle.w/2 - BOLA_RADIO;
    b->y = state.paddle.y + 1;
    if (b->vely > 0)
        b->vely = -b->vely;
    b->paddle = 0;
}

int actualizarBola(bola_t *b){
    if (b->paddle)
        return 0;

    b->x += b->velx;
    b->y += b->vely;

    if ( b->x - BOLA_RADIO <= 0 || b->x + BOLA_RADIO >= BORDER){
        // rebote de un lado
        b->velx = -b->velx;

        // devolver la pelota al espacio de juego
        if (b->x - BOLA_RADIO <= 0)
            b->x = 1 + BOLA_RADIO;
        else if (b->x + BOLA_RADIO >= BORDER)
            b->x = BORDER - BOLA_RADIO -1;
        return 1;
    }
    else if ( b->y - BOLA_RADIO <= 0 || b->y + BOLA_RADIO >= BORDER){
        // rebote arriba o abajo
        b->vely = -b->vely;

        // devolver la pelota al espacio de juego
        if (b->y - BOLA_RADIO <= 0)
            b->y = 1 + BOLA_RADIO;
        else if ( b->y + BOLA_RADIO >= BORDER)
            b->y = BORDER - BOLA_RADIO -1;
        return 1;
    } 
    else if ( state.paddle.x <= b->x  &&
              b->x <= state.paddle.x + state.paddle.w &&
              b->y >= state.paddle.y && b->y <= state.paddle.y + state.paddle.h){
        if(state.paddle.glue)
            b->paddle=1;

        if (b->vely > 0 && b->y > state.paddle.y){
            b->vely = -b->vely;
            b->y = state.paddle.y - BOLA_RADIO - 1;

            double magnitud = hypot(b->velx, b->vely);
            int xrebote = (state.paddle.x + state.paddle.w/2) - b->x;
            b->velx = - (0.85*magnitud)/(state.paddle.w/2) * xrebote;
            b->vely = -sqrt(magnitud*magnitud - b->velx*b->velx); 
      
        }
    } 
    else if (b->y > state.paddle.y + state.paddle.h + 1){
        // se murio
        b->paddle = 1;
        state.paddle.freeze = 0;
        state.lives--;
        b->velx = DEFAULT_BALL_SPEED_X;
        b->vely = DEFAULT_BALL_SPEED_Y;
        if(!state.lives)
            state.over=1;
        cleanExtras();
    
        return 1;
    }

    // pego con algun brick?
    int i;
    for (i=0; i<state.total_bricks; i++){
        brick *br = state.bricks+i;
        if (br->live){
            int hit = BrickHit(br, b);
            if (!hit) 
                continue;

            if (hit == 1){
                b->vely = -b->vely;
            } else if (hit == -1){
                b->velx = -b->velx;
            }

            state.score += SCORE_BRICK;
            br->live--;
            if(!br->live)
              state.left_bricks--;            
            // tiene extra?
            if(state.bricks[i].extra && !br->live){
                extra_t *e=&state.extras[i];
                e->x=8+state.bricks[i].x*31;
                e->y=1+50+state.bricks[i].y*16;
                e->extra=state.bricks[i].extra;
            }
            break;

        }
    }

    return 1;
}
