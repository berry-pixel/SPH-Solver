#include "../include/utilities.hpp"
#include "../include/constants.hpp"


particle makeParticle(sf::Vector2f pos, bool isStatic, sf::Color color)
{
    particle p;
    p.position = pos;
    p.mass = Constants::mass;
    p.radius = Constants::radius;
    p.isBoundary = isStatic;
    p.density = Constants::restDensity;
    p.color = color;
    return p;
}
