#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <cmath>
#include <iostream>
#include "../include/constants.hpp"
#include "../include/particle.hpp"
#include "../include/utilities.hpp"
#include "../include/SPH.hpp"
#include "../include/scene.hpp"
#include "../include/frame_recorder.hpp"

#include <iomanip>
#include <sstream>
#include <filesystem>
#include <imgui.h>
#include <imgui-SFML.h>

enum class EditorMode
{
    Brush,
    Line
};


int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "SPH");


    sf::RenderTexture renderTexture;
    renderTexture.resize({1200, 800});

    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    std::vector<particle> particles;

    FrameRecorder recorder;


    bool paused = true;
    bool recording = false;
    int frameNumber = 0;

    bool lineStartSet = false;
    EditorMode editorMode = EditorMode::Brush;
    sf::Vector2f lineStart;

    bool editing = false;
    bool placingBoundary = false;


    const sf::Font font("arial.ttf");
    sf::Text debugText(font);
    debugText.setCharacterSize(14);
    debugText.setFillColor(sf::Color::White);


    std::vector<Scene> scenes = createScenes();
    int selectedScene = 0;

    scenes[selectedScene].construct(particles);

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



    int trackedParticle = 0;



    float inputTimer = 0.f;


    float recordingTimer = 0.f;
    const float recordingInterval = 1.f / 30.f;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {


            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();

            if (editing &&
                editorMode == EditorMode::Line &&
                !ImGui::GetIO().WantCaptureMouse) {
                if (const auto* mousePressed =
                        event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Left) {
                        sf::Vector2i pixelPosition =
                            mousePressed->position;

                        sf::Vector2f clickPosition =
                            window.mapPixelToCoords(pixelPosition);

                        if (!lineStartSet) {
                            lineStart = clickPosition;
                            lineStartSet = true;
                        }
                        else {
                            sf::Vector2f lineEnd = clickPosition;
                            sf::Vector2f direction = lineEnd - lineStart;

                            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                            if (length > 0.f) {
                                    direction /= length;

                                    int particleCount = static_cast<int>( length / Constants::spacing );

                                    for (int i = 0; i <= particleCount; ++i) {
                                        sf::Vector2f position = lineStart + direction * (i * Constants::spacing);

                                        particle p = makeParticle(
                                            position,
                                            placingBoundary,
                                            placingBoundary
                                                ? sf::Color::White
                                                : sf::Color(119, 158, 203)
                                        );

                                        particles.push_back(p);
                                    }
                                }
                            lineStartSet = false;
                        }
                    }
                }
            }

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


        if (editing &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2f position = mouse;

            bool tooClose = false;

            for (const auto& p : particles)
            {
                float dx = p.position.x - position.x;
                float dy = p.position.y - position.y;

                float distanceSquared = dx * dx + dy * dy;

                float minDistance = Constants::spacing;

                if (distanceSquared < minDistance * minDistance)
                {
                    tooClose = true;
                    break;
                }
            }

            if (!tooClose)
            {
                particle p = makeParticle(
                    position,
                    placingBoundary,
                    placingBoundary
                        ? sf::Color::White
                        : sf::Color(119, 158, 203)
                );

                particles.push_back(p);
            }
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
        ImGui::Text("Scene");

        const char* sceneName = scenes[selectedScene].name.c_str();

        if (ImGui::BeginCombo("##Scene", sceneName))
        {
            for (int i = 0; i < scenes.size(); i++)
            {
                bool selected = (selectedScene == i);

                if (ImGui::Selectable(
                        scenes[i].name.c_str(),
                        selected))
                {
                    selectedScene = i;

                    particles.clear();
                    scenes[selectedScene].construct(particles);

                    paused = true;
                }

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::Text("Particles: %d", (int)particles.size());

        if (ImGui::Button(paused ? "Resume" : "Pause"))
        {
            paused = !paused;
        }

        if (ImGui::Button(recording ? "Stop Recording" : "Start Recording"))
        {
            recording = !recording;

            if (recording)
              {
                  frameNumber = 0;
                  recordingTimer = 0.f;

                  recorder.startRecording();
              }
        }

        ImGui::Text("Recorded Frames: %d", frameNumber);

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


        ImGui::SetNextWindowSize(
            ImVec2(350.f, 500.f),
            ImGuiCond_FirstUseEver
        );

        ImGui::Begin("Editing");

        if (ImGui::Checkbox("Edit Scene", &editing))
        {
            if (editing)
                paused = true;

            lineStartSet = false;
        }

        if (editing)
        {
            ImGui::Text("Editor Mode");

                if (ImGui::RadioButton(
                        "Brush",
                        editorMode == EditorMode::Brush)) {
                    editorMode = EditorMode::Brush;
                    lineStartSet = false;
                }

                ImGui::SameLine();

                if (ImGui::RadioButton(
                        "Line",
                        editorMode == EditorMode::Line)) {
                    editorMode = EditorMode::Line;
                    lineStartSet = false;
                }

                ImGui::Text("Particle Type");

                if (ImGui::RadioButton(
                        "Fluid",
                        !placingBoundary)) {
                    placingBoundary = false;
                }

                ImGui::SameLine();

                if (ImGui::RadioButton(
                        "Boundary",
                        placingBoundary)) {
                    placingBoundary = true;
                }
        }
        ImGui::End();

        // Render simulation to texture
        renderTexture.clear(CustomColors::Background);

        drawParticles(renderTexture, particles);

        renderTexture.display();


        if (recording && !paused)
        {
            recordingTimer += Constants::dt;

            if (recordingTimer >= recordingInterval)
            {
                sf::Image image =
                    renderTexture.getTexture().copyToImage();

                recorder.addFrame(
                    std::move(image),
                    frameNumber
                );

                frameNumber++;

                recordingTimer -= recordingInterval;
            }
        }


        window.clear(CustomColors::Background);

        sf::Sprite sprite(renderTexture.getTexture());
        window.draw(sprite);

        // drawParticles(window, particles);

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
