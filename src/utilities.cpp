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


void drawParticles(
    sf::RenderTarget& target,
    std::vector<particle>& particles
)
{
    sf::CircleShape c(Constants::radius);

    for (auto& p : particles)
    {
        c.setPosition({
            p.position.x - p.radius,
            p.position.y - p.radius
        });

        if (p.isBoundary)
        {
            c.setFillColor(p.color);
        }
        else
        {
            c.setFillColor(
                sf::Color(119, 158, 203)
            );
        }

        target.draw(c);
    }
}
