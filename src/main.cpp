#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include "../include/constants.hpp"
#include "../include/particle.hpp"
#include "../include/utilities.hpp"
#include "../include/SPH.hpp"
#include <imgui.h>
#include <imgui-SFML.h>


// Create a bucket like container.
void createContainer(std::vector<particle>& particles)
{
    float spacing = Constants::spacing;

    int width = 120;
    int height = 80;

    sf::Vector2f origin(250.f, 500.f);

    for (int layer = 0; layer < 3; layer++)
    {
        for (int i = 0; i < width; i++)
        {
            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f(i * spacing, layer * spacing),
                    true,
                    CustomColors::Boundary
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
                    CustomColors::Boundary
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
                    CustomColors::Boundary
                )
            );
        }
    }
}

void createFluid( std::vector<particle>& particles ) {

    int cols = 40;
    int rows = 40; // 50 particles total

    sf::Vector2f start(260.f, 493.f);

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            particles.push_back(
                makeParticle(
                    start + sf::Vector2f(x * Constants::spacing , - y * Constants::spacing),
                    false,
                    CustomColors::Fluid
                )
            );
        }
    }
}

void createContainerWithFunnel(std::vector<particle>& particles)
{
    float spacing = Constants::spacing;

    int width = 180;
    int height = 90;

    sf::Vector2f origin(170.f, 500.f);

    // Bottom
    for (int layer = 0; layer < 2; layer++)
        for (int i = 0; i < width; i++)
            particles.push_back(makeParticle(
                origin + sf::Vector2f(i*spacing,
                                      layer*spacing),
                true,
                CustomColors::Boundary));

    // Side walls
    for (int layer = 0; layer < 2; layer++)
    {
        for (int i = 0; i < height; i++)
        {
            particles.push_back(makeParticle(
                origin + sf::Vector2f(layer*spacing,
                                      -i*spacing),
                true,
                CustomColors::Boundary));

            particles.push_back(makeParticle(
                origin + sf::Vector2f((width-1)*spacing-layer*spacing,
                                      -i*spacing),
                true,
                CustomColors::Boundary));
        }
    }

    int funnelTop = 45;      // height where funnel starts
    int funnelDepth = 25;    // how far down it goes

    int funnelWidth = 50;    // width of the opening
    int funnelCenter = width / 2;

    // Left slope: wide top -> narrow bottom
    for (int i = 0; i <= funnelDepth; i++)
    {
        float t = float(i) / funnelDepth;

        int x = static_cast<int>(
            funnelCenter - funnelWidth / 2 +
            (funnelWidth / 2) * t
        );

        particles.push_back(
            makeParticle(
                origin + sf::Vector2f(
                    x * spacing,
                    -(funnelTop + i) * spacing
                ),
                true,
                CustomColors::Boundary
            )
        );
    }


    // Right slope: wide top -> narrow bottom
    for (int i = 0; i <= funnelDepth; i++)
    {
        float t = float(i) / funnelDepth;

        int x = static_cast<int>(
            funnelCenter + funnelWidth / 2 -
            (funnelWidth / 2) * t
        );

        particles.push_back(
            makeParticle(
                origin + sf::Vector2f(
                    x * spacing,
                    -(funnelTop + i) * spacing
                ),
                true,
                CustomColors::Boundary
            )
        );
    }
}

void createFluidAboveFunnel(std::vector<particle>& particles)
{
    int cols = 60;
    int rows = 30;

    sf::Vector2f start(350.f, 250.f);

    for (int y = 0; y < rows; y++)
        for (int x = 0; x < cols; x++)
            particles.push_back(makeParticle(
                start +
                sf::Vector2f(x*Constants::spacing,
                            -y*Constants::spacing),
                false,
                CustomColors::Fluid));
}

void createContainerWithPlatforms(std::vector<particle>& particles)
{
    float spacing = Constants::spacing;

    int width = 120;
    int height = 80;

    sf::Vector2f origin(250.f, 500.f);

    // Bottom
    for (int layer = 0; layer < 3; layer++)
        for (int i = 0; i < width; i++)
            particles.push_back(makeParticle(
                origin + sf::Vector2f(i * spacing, layer * spacing),
                true,
                CustomColors::Boundary));

    // Left wall
    for (int layer = 0; layer < 2; layer++)
        for (int i = 0; i < height; i++)
            particles.push_back(makeParticle(
                origin + sf::Vector2f(layer * spacing, -i * spacing),
                true,
                CustomColors::Boundary));

    // Right wall
    for (int layer = 0; layer < 2; layer++)
        for (int i = 0; i < height; i++)
            particles.push_back(makeParticle(
                origin + sf::Vector2f((width-1)*spacing-layer*spacing,
                                      -i*spacing),
                true,
                CustomColors::Boundary));

    int platformLength = width / 3;

    // Left platform
    int leftHeight = 25;
    for (int layer = 0; layer < 2; layer++)
        for (int i = 0; i < platformLength; i++)
            particles.push_back(makeParticle(
                origin + sf::Vector2f(i*spacing,
                                      -leftHeight*spacing-layer*spacing),
                true,
                CustomColors::Boundary));

    // Right platform
    int rightHeight = 50;
    for (int layer = 0; layer < 2; layer++)
        for (int i = 0; i < platformLength; i++)
            particles.push_back(makeParticle(
                origin + sf::Vector2f((width-platformLength+i)*spacing,
                                      -rightHeight*spacing-layer*spacing),
                true,
                CustomColors::Boundary));
}

void createFluidOnPlatforms(std::vector<particle>& particles)
{
    int cols = 25;
    int rows = 25;

    // Above left platform
    sf::Vector2f leftStart(
        260.f,
        500.f - 25 * Constants::spacing - 2 * Constants::spacing);

    for (int y = 0; y < rows; y++)
        for (int x = 0; x < cols; x++)
            particles.push_back(makeParticle(
                leftStart +
                sf::Vector2f(x*Constants::spacing,
                            -y*Constants::spacing),
                false,
                CustomColors::Fluid));

    // Above right platform
    sf::Vector2f rightStart(
        250.f + (120-cols-2)*Constants::spacing,
        500.f - 50 * Constants::spacing - 2 * Constants::spacing);

    for (int y = 0; y < rows; y++)
        for (int x = 0; x < cols; x++)
            particles.push_back(makeParticle(
                rightStart +
                sf::Vector2f(x*Constants::spacing,
                            -y*Constants::spacing),
                false,
                CustomColors::Fluid));
}


void createContainerWithBarrier(std::vector<particle>& particles)
{
    float spacing = Constants::spacing;

    int width = 130;
    int height = 80;

    sf::Vector2f origin(250.f, 500.f);

    for (int layer = 0; layer < 3; layer++)
    {
        for (int i = 0; i < width; i++)
        {
            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f(i * spacing,
                                          layer * spacing),
                    true,
                    CustomColors::Boundary
                )
            );
        }
    }

    // Left wall
    for (int layer = 0; layer < 2; layer++)
    {
        for (int i = 0; i < height; i++)
        {
            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f(layer * spacing,
                                          -i * spacing),
                    true,
                    CustomColors::Boundary
                )
            );
        }
    }

    // Right wall
    for (int layer = 0; layer < 2; layer++)
    {
        for (int i = 0; i < height; i++)
        {
            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f(
                        (width - 1) * spacing - layer * spacing,
                        -i * spacing
                    ),
                    true,
                    CustomColors::Boundary
                )
            );
        }
    }

    int barrierX = width / 2;

    int holeSize = 4;
    int holeStart = 1;

    for (int layer = 0; layer < 2; layer++)
    {
        for (int i = 0; i < height; i++)
        {
            if (i >= holeStart &&
                i < holeStart + holeSize)
            {
                continue;
            }

            particles.push_back(
                makeParticle(
                    origin + sf::Vector2f(
                        barrierX * spacing + layer * spacing,
                        -i * spacing
                    ),
                    true,
                    CustomColors::Boundary
                )
            );
        }
    }
}

void createFluidLeftOfBarrier(std::vector<particle>& particles)
{
    int cols = 50;
    int rows = 35;

    sf::Vector2f start(260.f, 493.f);

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            particles.push_back(
                makeParticle(
                    start + sf::Vector2f(
                        x * Constants::spacing,
                        -y * Constants::spacing
                    ),
                    false,
                    CustomColors::Fluid
                )
            );
        }
    }
}





int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "SPH");
    
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    std::vector<particle> particles;


    bool paused = true;

    const sf::Font font("arial.ttf");
    sf::Text debugText(font);
    debugText.setCharacterSize(14);
    debugText.setFillColor(sf::Color::White);


    createFluid(particles);
    createContainer(particles);

    // createContainerWithBarrier(particles);
    // createFluidLeftOfBarrier(particles);

    // createContainerWithPlatforms(particles);
    // createFluidOnPlatforms(particles);



    int trackedParticle = 0;



    // particle falling;
    // falling = makeParticle(
    //     {250.f, 200.f},
    //     false,
    //     sf::Color::Red,
    //     params.mass,
    //     params
    // );

    // particles.push_back(falling);




    // sf::Vector2f start(200.f, 300.f);
    // for (int row = 0; row < 2; row++)
    // {
    //     for (int col = 0; col < 20; col++)
    //     {
    //         particle p;
    //         p = makeParticle(
    //             start + sf::Vector2f(col * params.spacing, row * params.spacing),
    //             true,
    //             sf::Color::Green,
    //             params.mass,
    //             params
    //         );


    //         particles.push_back(p);
    //     }
    // }

    float inputTimer = 0.f;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {


            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();

        }

        ImGui::SFML::Update(window, deltaClock.restart());


        inputTimer += Constants::dt;

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

            float dist = (dx*dx + dy*dy);

            if (dist < bestDist * bestDist)
            {
                bestDist = dist;
                hovered = &p;
            }
        }



        // window.setKeyRepeatEnabled(false);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)){
            paused = 1;
            inputTimer = 0.f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)){
            paused = 0;
            inputTimer = 0.f;
        }



        if(paused == 0){
            // findNeighbours(particles);

            findNeighboursGridSearch(particles);


            calculateDensity(particles);

            calculatePressure(particles);

            calculatePressureAcceleration(particles);
            calculateViscosityAccelration(particles);


            for (auto& p : particles)
            {
                if (p.isBoundary) continue;

                sf::Vector2f acceleration =
                    p.pressureAcceleration +
                    p.viscosityAcceleration +
                    Constants::gravity;

                p.velocity += acceleration * Constants::dt;

                p.position += p.velocity * Constants::dt;



            }
        }

        // IMgui window
        ImGui::Begin("Simulation");

        ImGui::Text("Particles: %d", (int)particles.size());

        if (ImGui::Button(paused ? "Resume" : "Pause"))
        {
            paused = !paused;
        }

        ImGui::SliderFloat("Gravity",
                        &Constants::gravity.y,
                        -2000.f,
                        2000.f);

        ImGui::SliderFloat("dt",
                        &Constants::dt,
                        0.0001f,
                        0.02f);

        ImGui::SliderFloat("Viscosity",
                        &Constants::viscosity,
                        0.f,
                        10.f);

        ImGui::End();

        window.clear(CustomColors::Background);


        sf::CircleShape c(Constants::radius);
        for (auto& p : particles)
        {

            c.setPosition({
                p.position.x - p.radius,
                p.position.y - p.radius
            });

            if (p.isBoundary)
                c.setFillColor(p.color);
            else{
                // float ratio = std::clamp(
                //     p.density / params.restDensity,
                //     0.f,
                //     2.f
                // );

                // int red = static_cast<int>(255.f * ratio / 2.f);
                // int blue = static_cast<int>(255.f * (1.f - ratio / 2.f));
                // c.setFillColor(
                //     sf::Color(red, 0, blue)
                // );
                c.setFillColor(
                    sf::Color(119, 158, 203)
                );

            }

            window.draw(c);
        }

        // std::cout
        //   << "rho = " << particles[0].density
        //   << " expected rho0 = " << params.restDensity
        //   << '\n';

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

        particle& p = particles[trackedParticle];

        std::stringstream ss;

        ss << std::fixed << std::setprecision(4);

        ss << "Tracked Particle\n";
        ss << "----------------\n";

        ss << "Density:  " << p.density << '\n';
        ss << "Pressure: " << p.pressure << '\n';

        ss << "Pos X:    " << p.position.x << '\n';
        ss << "Pos Y:    " << p.position.y << '\n';

        ss << "Vel X:    " << p.velocity.x << '\n';
        ss << "Vel Y:    " << p.velocity.y << '\n';

        ss << "Pressure Acc Y: "
           << p.pressureAcceleration.y << '\n';

        ss << "Viscosity Acc Y: "
           << p.viscosityAcceleration.y << '\n';

        ss << "Neighbours: "
           << p.neighbors.size() << '\n';

        for (size_t i = 0; i < p.neighbors.size(); ++i)
        {
            ss << p.neighbors[i];
            if (i + 1 < p.neighbors.size())
                ss << ", ";
        }

        debugText.setString(ss.str());
        debugText.setPosition({850.f, 50.f});

        window.draw(debugText);


        ImGui::SFML::Render(window);

        window.display();
    }
}
