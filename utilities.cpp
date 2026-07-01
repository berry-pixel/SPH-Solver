#include "utilities.h"


particle makeParticle(sf::Vector2f pos, bool isStatic, sf::Color color, float mass, SPHParameters params)
{
    particle p;
    p.position = pos;
    p.mass = mass;
    p.radius = params.h/2;
    p.isStatic = isStatic;
    p.density = params.restDensity;
    p.color = color;
    return p;
}
