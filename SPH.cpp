#include "SPH.h"


float distance(sf::Vector2f xi, sf::Vector2f xj) {

    float dx = xi.x - xj.x;
    float dy = xi.y - xj.y;

    return sqrt((dx * dx) + (dy * dy));

}


// Kernel functions
float Kernel(sf::Vector2f xi, sf::Vector2f xj, float h) {

    float distance_xi_xj = distance(xi, xj);

    float alpha = (5/(14.0f * M_PI * h * h));

    float d = distance_xi_xj/h;

    float t1 = (1 - d) > 0 ? (1-d) : 0;
    float t2 = (2 - d) > 0 ? (2-d) : 0;

    float kernel = alpha * (t2*t2*t2 - 4*t1*t1*t1);

    return kernel;
}

sf::Vector2f firstDerivativeKernel(sf::Vector2f xi, sf::Vector2f xj, float h) {

    float r = distance(xi, xj);


    if (r < 1e-6f)
    {
        return sf::Vector2f(0.f, 0.f);
    }


    float alpha = (5/(14.0f * M_PI * h * h));

    float d = r/h;


    float t1 = (1 - d) > 0 ? (1-d) : 0;
    float t2 = (2 - d) > 0 ? (2-d) : 0;

    sf::Vector2f w1 = alpha * ((xi - xj) / (d * h * h)) * (-3 * t2 * t2 + 12 * t1 * t1);

    return w1;

}


// Calculations
void calculateDensity (std::vector<particle>& particles, float h) {
    for(particle& p: particles) {

        float density = 0.0f;

        for (int neighborIndex : p.neighbors) {

            particle& neighbor = particles[neighborIndex];

            density = density + neighbor.mass * Kernel(p.position, neighbor.position, h);

        }

        p.density = density;

    }
}

void calculatePressure (std::vector<particle>& particles, float stiffnessCoefficient, float restDensity ) {

    for(particle& p: particles) {

        p.pressure = stiffnessCoefficient * ((p.density / restDensity) - 1);

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
                particles[i].neighbors.push_back(j);
            }
        }
    }
}
