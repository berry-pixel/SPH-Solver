#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include "particle.h"
#include "SPH.h"
#include "utilities.h"

struct SPHParameters
{
    float spacing;
    float dt;
    float kernel_support;

    float restDensity;

    float stiffness;

    float viscosity;

    sf::Vector2f gravity;
};



void createContainer(std::vector<particle>& particles, float mass, SPHParameters params)
{
    float spacing = 7.f;

    int width = 60;
    int height = 80;

    sf::Vector2f origin(200.f, 500.f);

    for (int layer = 0; layer < 2; layer++)
    {
        for (int i = 0; i < width; i++)
        {
            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f(i * spacing, layer * spacing),
                    true,
                    sf::Color::Blue,
                    mass
                )
            );
        }
    }

    for (int layer = 0; layer < 2; layer++)
    {
        for (int i = 0; i < height; i++)
        {
            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f(layer * spacing, -i * spacing),
                    true,
                    sf::Color::Blue,
                    mass
                )
            );
        }
    }

    for (int layer = 0; layer < 2; layer++)
    {
        for (int i = 0; i < height; i++)
        {
            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f((width - 1) * spacing - layer * spacing,
                                          -i * spacing),
                    true,
                    sf::Color::Blue,
                    mass
                )
            );
        }
    }
}

void createFluid(std::vector<particle>& particles, float mass, SPHParameters params)
{
    float spacing =5.f;

    int cols = 20;
    int rows = 20; // 50 particles total

    sf::Vector2f start(260.f, 200.f);

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            particles.push_back(
                makeParticle(
                    start + sf::Vector2f(x * spacing, y * spacing),
                    false,
                    sf::Color::Red,
                    mass
                )
            );
        }
    }
}





int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "SPH");

    std::vector<particle> particles;


    bool paused = false;

    const sf::Font font("arial.ttf");
    sf::Text debugText(font);
    debugText.setCharacterSize(14);
    debugText.setFillColor(sf::Color::White);




    SPHParameters params
    {
        .spacing = 8.f,
        .dt = 0.0009f,
        .kernel_support = 1.5f * params.spacing,
        .restDensity = 0.1f,
        .stiffness = 600.f,
        .viscosity = 300.f,
        .gravity = {0.f, 10.f}
    };


    float mass = params.restDensity * params.spacing * params.spacing;




    // createContainer(particles, mass);
    // createFluid(particles, mass);



    particle falling;
    falling = makeParticle(
        {300.f, 200.f},
        false,
        sf::Color::Red,
        mass
    );

    particles.push_back(falling);

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 30; col++)
        {
            particle p;
            p = makeParticle(
                 {
                200.f + col * params.spacing,
                300.f + row * params.spacing
                },
                true,
                sf::Color::Blue,
                mass
            );


            particles.push_back(p);
        }
    }

    float inputTimer = 0.f;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        inputTimer += params.dt;

        sf::Vector2i mousePixel =
            sf::Mouse::getPosition(window);

        sf::Vector2f mouse =
            window.mapPixelToCoords(mousePixel);

        particle* hovered = nullptr;

        float bestDist = 20.f;

        for (auto& p : particles)
        {
            float dx = p.position.x - mouse.x;
            float dy = p.position.y - mouse.y;

            float dist = std::sqrt(dx*dx + dy*dy);

            if (dist < bestDist)
            {
                bestDist = dist;
                hovered = &p;
            }
        }


        if (inputTimer > 0.1f)

        window.setKeyRepeatEnabled(false);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)){
            paused = 1;
            inputTimer = 0.f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)){
            paused = 0;
            inputTimer = 0.f;
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))) {
            params.dt = params.dt - 0.001f;
            inputTimer = 0.f;
            std::cout<<"\nDT: "<<params.dt;
        } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))) {
            params.dt = params.dt + 0.001f;
            inputTimer = 0.f;

        } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))) {
            params.stiffness = params.stiffness - 1.0f;
            inputTimer = 0.f;
        } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))) {
            params.stiffness = params.stiffness + 1.0f;
            inputTimer = 0.f;
        } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))) {
            params.viscosity = params.viscosity - 1.0f;
            inputTimer = 0.f;
        } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))) {
            params.viscosity = params.viscosity + 1.0f;
            inputTimer = 0.f;
        }


        if(paused == 0){
            findNeighbours(params.kernel_support, particles);

            calculateDensity(particles, params.kernel_support);

            calculatePressure(particles, params.stiffness, params.restDensity);

            calculatePressureAcceleration(particles, params.kernel_support);
            calculateViscosityAccelration(particles, params.kernel_support, params.viscosity);

            // std::cout<<"RUnning calcs\n";

            for (auto& p : particles)
            {
                if (p.isStatic) continue;

                sf::Vector2f acceleration =
                    p.pressureAcceleration +
                    p.viscosityAcceleration +
                    params.gravity;

                p.velocity += acceleration * params.dt;

                p.position += p.velocity * params.dt;



            }
        }

        // 7. render
        window.clear();

        for (auto& p : particles)
        {
            sf::CircleShape c(p.radius);

            c.setPosition({
                p.position.x - p.radius,
                p.position.y - p.radius
            });

            if (p.isStatic)
                c.setFillColor(p.color);
            else
                c.setFillColor(p.color);

            window.draw(c);
        }

        if (hovered)
        {
            std::stringstream ss;

            ss << "Density: " << hovered->density << '\n'
               << "Pressure: " << hovered->pressure << '\n'
               << "Mass: " << hovered->mass << '\n'
               << "Neighbours: "
               << hovered->neighbors.size() << '\n'
               << "Velocity: ("
               << hovered->velocity.x << ", "
               << hovered->velocity.y << ")";

            debugText.setString(ss.str());

            debugText.setPosition(
                hovered->position +
                sf::Vector2f(15.f, -15.f)
            );

            window.draw(debugText);
        }


        window.display();
    }
}
