#ifndef SPH_H
#define SPH_H

#include <SFML/Graphics.hpp>
#include "particle.h"

// Utility functions
float distance (sf::Vector2f xi, sf::Vector2f xj);

// Kernel Functions
float Kernel(sf::Vector2f xi, sf::Vector2f xj, float h);
sf::Vector2f firstDerivativeKernel(sf::Vector2f xi, sf::Vector2f xj, float h);

// SPH Calculations
void calculateDensity  ( std::vector<particle>& particles, float h );
void calculatePressure ( std::vector<particle>& particles, float stiffnessCoefficient, float restDensity );
void calculatePressureAcceleration (std::vector<particle>& particles, float h);
void calculateViscosityAccelration (std::vector<particle>& particles, float h, float viscosityCoefficient);

// Neighbour search
void findNeighbours( float smoothing_radius, std::vector<particle>& particles );

#endif