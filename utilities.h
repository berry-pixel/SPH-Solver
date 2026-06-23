#ifndef UTILITIES_H
#define UTILITIES_H

#include "particle.h"

particle makeParticle(sf::Vector2f position, bool isStatic, sf::Color color, float mass);

void setFluidColor(particle p, sf::Color color);

void input_controller();


#endif