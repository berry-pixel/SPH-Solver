#ifndef PARTICLE_H
#define PARTICLE_H


#include<SFML/Graphics.hpp>

class particle {
    public:
        sf::Vector2f position;
        sf::Vector2f velocity{0.0f, 0.0f};


        sf::Color color;
        float radius;

        float mass;
        float density;
        float pressure;

        sf::Vector2f pressureAcceleration{0.0f, 0.0f};
        sf::Vector2f viscosityAcceleration{0.0f, 0.0f};

        bool isStatic = false;

        std::vector<int> neighbors;

};





#endif