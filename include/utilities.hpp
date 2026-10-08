#ifndef UTILITIES_H
#define UTILITIES_H

#include "particle.hpp"

particle makeParticle(sf::Vector2f position, bool isStatic, sf::Color color);
void setFluidColor(particle p, sf::Color color);
void input_controller();

void drawParticles(
    sf::RenderTarget& target,
    std::vector<particle>& particles
);

#endif
