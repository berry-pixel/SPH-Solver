#ifndef UTILITIES_H
#define UTILITIES_H

#include "particle.h"

struct SPHParameters
{
    float h;
    float spacing;
    float dt;
    float kernel_support;

    float restDensity;

    float stiffness;

    float viscosity;

    sf::Vector2f gravity;
    float mass;
};



particle makeParticle(sf::Vector2f position, bool isStatic, sf::Color color, float mass, SPHParameters params);

void setFluidColor(particle p, sf::Color color);

void input_controller();




#endif
