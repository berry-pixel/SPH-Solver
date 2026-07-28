#include "../include/SPH.hpp"
#include "../include/constants.hpp"
#include "../include/particle.hpp"
#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <iostream>
#include <vector>


float distance(sf::Vector2f xi, sf::Vector2f xj) {

    float dx = xi.x - xj.x;
    float dy = xi.y - xj.y;

    return std::sqrt(dx * dx + dy * dy);

}


// Kernel functions
float Kernel(sf::Vector2f xi, sf::Vector2f xj) {

    float distance_xi_xj = distance(xi, xj);


    float q = distance_xi_xj/Constants::spacing;



    float t = 0;

    if (0 <= q && q < 1) {

        t = (2 - q) * (2 - q) * (2 - q)  - 4 * (1-q) * (1-q) * (1-q);

    } else if (1 <= q && q < 2) {
        t = (2 - q) * (2 - q) * (2 - q);
    } else {
        t = 0;
    }


    float kernel = Constants::kernelAlpha * t;


    return kernel;
}

sf::Vector2f firstDerivativeKernel(sf::Vector2f xi, sf::Vector2f xj) {

    float d = distance(xi, xj);

    float h = Constants::spacing;

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
void calculateDensity (std::vector<particle>& particles) {

    for(particle& p: particles) {

        float density = 0.0f;


        for (int neighborIndex : p.neighbors) {

            particle& neighbor = particles[neighborIndex];

            density = density + neighbor.mass * Kernel(p.position, neighbor.position);

        }

        p.density = density;


    }
}

void calculatePressure ( std::vector<particle>& particles ) {

    for(particle& p: particles) {

        float pressure = Constants::stiffness * ((p.density / Constants::restDensity) - 1);

        p.pressure = pressure;

        p.pressure = std::max(0.f, pressure);

    }

}


void calculatePressureAcceleration (std::vector<particle>& particles) {

    float h = Constants::spacing;

    for(particle& p: particles) {

        sf::Vector2f pressureAcceleration{0.0f, 0.0f};

        if (p.isBoundary == false) {

            for (int neighborIndex : p.neighbors) {

                particle& neighbor = particles[neighborIndex];

                float neighborDensity;
                float neighborPressure;

                if ( neighbor.isBoundary ) {

                    neighborDensity = p.density;
                    neighborPressure = p.pressure;

                } else {

                    neighborDensity = neighbor.density;
                    neighborPressure = neighbor.pressure;

                }

                float pressureTerm = (p.pressure/(p.density * p.density)) + (neighborPressure/(neighborDensity * neighborDensity));

                pressureAcceleration = pressureAcceleration + (-neighbor.mass * pressureTerm * firstDerivativeKernel(p.position, neighbor.position));

           }

            p.pressureAcceleration = pressureAcceleration;
        }


    }

}

void calculateViscosityAccelration (std::vector<particle>& particles) {

     for(particle& p: particles) {

        sf::Vector2f viscosityAccelration{0.0f, 0.0f};

        for (int neighborIndex : p.neighbors) {

            particle& neighbor = particles[neighborIndex];

            sf::Vector2f delta_velocity = (p.velocity - neighbor.velocity);
            sf::Vector2f delta_position = p.position - neighbor.position;
            float dot_product = delta_velocity.x * delta_position.x + delta_velocity.y * delta_position.y;
            float position_dot_product = delta_position.x * delta_position.x + delta_position.y * delta_position.y;


            float scalar_term = (Constants::viscosity * (neighbor.mass/neighbor.density) * (dot_product/(position_dot_product + 0.01f * Constants::spacing * Constants::spacing)));

            viscosityAccelration += scalar_term * firstDerivativeKernel(p.position, neighbor.position);

        }

        p.viscosityAcceleration = viscosityAccelration;

    }

}

// Find neighbours
void findNeighbours(
    std::vector<particle>& particles
) {
    float smoothingRadiusSquared =
        Constants::kernelSupport * Constants::kernelSupport;

    for (auto& particle : particles)
    {
        particle.neighbors.clear();
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
     //               particles[j].color = sf::Color::White;
                }

                particles[i].neighbors.push_back(j);
            }

        }
    }
}

void findNeighboursGridSearch(std::vector<particle>& particles)
{
    const float cellSize = Constants::kernelSupport;
    const float supportSquared = cellSize * cellSize;

    const int gridWidth =
        std::ceil(Constants::windowWidth / cellSize);

    const int gridHeight =
        std::ceil(Constants::windowHeight / cellSize);

    struct Cell
    {
        std::vector<int> particles;
    };

    std::vector<Cell> grid(gridWidth * gridHeight);

    // Clear neighbour lists
    for (auto& p : particles)
        p.neighbors.clear();

    // Insert particles into grid
    for (int i = 0; i < particles.size(); i++)
    {
        int cx = particles[i].position.x / cellSize;
        int cy = particles[i].position.y / cellSize;

        if (cx < 0 || cx >= gridWidth ||
            cy < 0 || cy >= gridHeight)
        {
            continue;
        }


        grid[cy * gridWidth + cx].particles.push_back(i);
    }

    // Find neighbours
    for (int i = 0; i < particles.size(); i++)
    {
        int cx = particles[i].position.x / cellSize;
        int cy = particles[i].position.y / cellSize;



        for (int dy = -1; dy <= 1; dy++)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                int nx = cx + dx;
                int ny = cy + dy;

                if (nx < 0 || nx >= gridWidth ||
                    ny < 0 || ny >= gridHeight)
                    continue;

                Cell& cell = grid[ny * gridWidth + nx];

                for (int j : cell.particles)
                {
                    if (i == j)
                    {
                        particles[i].neighbors.push_back(j);
                        continue;
                    }

                    sf::Vector2f delta =
                        particles[j].position -
                        particles[i].position;

                    float distSquared =
                        delta.x * delta.x +
                        delta.y * delta.y;

                    if (distSquared < supportSquared)
                        particles[i].neighbors.push_back(j);
                }
            }
        }
    }
}
