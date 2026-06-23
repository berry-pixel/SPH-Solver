#include "utilities.h"

particle makeParticle(sf::Vector2f pos, bool isStatic, sf::Color color, float mass)
{
    particle p;
    p.position = pos;
    p.mass = mass;
    p.radius = 6.f;
    p.isStatic = isStatic;
    p.color = color;
    return p;
}