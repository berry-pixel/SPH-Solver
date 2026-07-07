#ifndef SPH_H
#define SPH_H

#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "utilities.hpp"

// Utility functions
float distance (sf::Vector2f xi, sf::Vector2f xj);

// Kernel Functions
float Kernel( sf::Vector2f xi, sf::Vector2f xj );
sf::Vector2f firstDerivativeKernel( sf::Vector2f xi, sf::Vector2f xj );

// SPH Calculations
void calculateDensity  ( std::vector<particle>& particles );
void calculatePressure ( std::vector<particle>& particles );
void calculatePressureAcceleration (std::vector<particle>& particles );
void calculateViscosityAccelration (std::vector<particle>& particles );

// Neighbour search
void findNeighbours( std::vector<particle>& particles );

#endif
