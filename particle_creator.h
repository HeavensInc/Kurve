#ifndef PARTICLE_CREATOR_H
#define PARTICLE_CREATOR_H
#include "particles.h"
#include "constants.h"

particle* pt_create_start(float r,float g,float b,
                float x,float y,
                float winkel);

particle* pt_create_dot(float r,float g,float b,
                float x,float y,
                float winkel);
                
particle* pt_create_punkt(float r,float g,float b,
                float x,float y);

#endif // PARTICLE_CREATOR_H
