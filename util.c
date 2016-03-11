#include "util.h"

void setMaterial(material *m){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m->specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, m->shininess);
}
