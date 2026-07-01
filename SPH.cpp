#include "SPH.h"
#include "particle.h"
#include "utilities.h"
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>


float distance(sf::Vector2f xi, sf::Vector2f xj) {

    float dx = xi.x - xj.x;
    float dy = xi.y - xj.y;

    return sqrt((dx * dx) + (dy * dy));

}


// Kernel functions
float Kernel(sf::Vector2f xi, sf::Vector2f xj, float h) {

    float distance_xi_xj = distance(xi, xj);

    float alpha = (5/(14.0f * M_PI));


    float q = distance_xi_xj/h;



    float t = 0;

    if (0 <= q && q < 1) {

        t = (2 - q) * (2 - q) * (2 - q)  - 4 * (1-q) * (1-q) * (1-q);

    } else if (1 <= q && q < 2) {
        t = (2 - q) * (2 - q) * (2 - q);
    } else {
        t = 0;
    }


    float kernel = alpha * t / (h * h);


    std::cout<<"\n kernel debugging :"<<kernel;
    return kernel;
}

sf::Vector2f firstDerivativeKernel(sf::Vector2f xi, sf::Vector2f xj, float h) {

    float d = distance(xi, xj);


    if (d < 1e-6f)
    {
        return sf::Vector2f(0.f, 0.f);
    }


    float alpha = (5/(14.0f * M_PI * h * h));

    float q = d/h;

    float t = 0;

    if (0 <= q && q < 1) {

        t = -3 * (2 - q) * (2 - q)  + 12 * (1-q) * (1-q);

    } else if (1 <= q && q < 2) {
        t = -3 * (2 - q) * (2 - q);
    } else {
        t = 0;
    }


    sf::Vector2f w1 = alpha * ((xi - xj) / (d  * h )) * t;



    return w1;

}


// Calculations
void calculateDensity (std::vector<particle>& particles, float h, SPHParameters params) {

    std::cout<<"\n Debugging particles mass: "<< params.mass;
    for(particle& p: particles) {

        float density = 0.0f;

        for (int neighborIndex : p.neighbors) {

            particle& neighbor = particles[neighborIndex];
            std::cout<<"\nDebugging neighbours: " << neighborIndex;

            density = density + neighbor.mass * Kernel(p.position, neighbor.position, params.spacing);

        }

        p.density = density;


    }
}

void calculatePressure (std::vector<particle>& particles, float stiffnessCoefficient, float restDensity ) {




    for(particle& p: particles) {

        float pressure = stiffnessCoefficient * ((p.density / restDensity) - 1);

        p.pressure = pressure;

        p.pressure = std::max(0.f, pressure);

    }

}


void calculatePressureAcceleration (std::vector<particle>& particles, float h) {

    for(particle& p: particles) {

        sf::Vector2f pressureAcceleration{0.0f, 0.0f};

        for (int neighborIndex : p.neighbors) {

            particle& neighbor = particles[neighborIndex];

            float pressureTerm = (p.pressure/(p.density * p.density)) + (neighbor.pressure/(neighbor.density * neighbor.density));

            pressureAcceleration = pressureAcceleration + (-neighbor.mass * pressureTerm * firstDerivativeKernel(p.position, neighbor.position, h));

        }

        p.pressureAcceleration = pressureAcceleration;

    }

}

void calculateViscosityAccelration (std::vector<particle>& particles, float h, float viscosityCoefficient) {

     for(particle& p: particles) {

        sf::Vector2f viscosityAccelration{0.0f, 0.0f};

        for (int neighborIndex : p.neighbors) {

            particle& neighbor = particles[neighborIndex];

            sf::Vector2f delta_velocity = (p.velocity - neighbor.velocity);
            sf::Vector2f delta_position = p.position - neighbor.position;
            float dot_product = delta_velocity.x * delta_position.x + delta_velocity.y * delta_position.y;
            float position_dot_product = delta_position.x * delta_position.x + delta_position.y * delta_position.y;


            float scalar_term = (viscosityCoefficient * (neighbor.mass/neighbor.density) * (dot_product/(position_dot_product + 0.01f * h * h)));

            viscosityAccelration += scalar_term * firstDerivativeKernel(p.position, neighbor.position, h);

        }

        p.viscosityAcceleration = viscosityAccelration;

    }

}

// Find neighbours
void findNeighbours(
    float smoothing_radius,
    std::vector<particle>& particles
) {
    float smoothingRadiusSquared =
        smoothing_radius * smoothing_radius;

    for (auto& particle : particles)
    {
        particle.neighbors.clear();
        if(particle.isStatic) {
            particle.color = sf::Color::Green;
        } else {
            particle.color = sf::Color::Blue;
        }
    }

    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles.size(); j++)
        {
            if (i == j) {
                particles[i].neighbors.push_back(j);
                continue;
            }

            sf::Vector2f delta =
                particles[j].position -
                particles[i].position;

            float distSquared =
                delta.x * delta.x +
                delta.y * delta.y;


            if (distSquared < smoothingRadiusSquared)
            {


                // for debugging:
                if (i == 0) {
                    particles[j].color = sf::Color::White;
                }

                particles[i].neighbors.push_back(j);
            }

        }
    }
}
