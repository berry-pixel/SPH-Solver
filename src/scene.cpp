#include "../include/particle.hpp"
#include "../include/scene.hpp"
#include "../include/constants.hpp"
#include "../include/utilities.hpp"

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


std::vector<Scene> createScenes()
{
    std::vector<Scene> scenes;

    scenes.push_back({
        "Bucket",
        [](std::vector<particle>& particles)
        {
            createFluid(particles);
            createContainer(particles);
        }
    });

    scenes.push_back({
        "Barrier",
        [](std::vector<particle>& particles)
        {
            createContainerWithBarrier(particles);
            createFluidLeftOfBarrier(particles);
        }
    });

    scenes.push_back({
        "Funnel",
        [](std::vector<particle>& particles)
        {
            createContainerWithFunnel(particles);
            createFluidAboveFunnel(particles);
        }
    });

    scenes.push_back({
        "Platforms",
        [](std::vector<particle>& particles)
        {
            createContainerWithPlatforms(particles);
            createFluidOnPlatforms(particles);
        }
    });




    return scenes;
}
