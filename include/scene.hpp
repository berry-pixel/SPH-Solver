#pragma once

#include <string>
#include <vector>
#include <functional>


#include "particle.hpp"

struct Scene
{
    std::string name;
    std::function<void(std::vector<particle>&)> construct;
};


void createContainer(std::vector<particle>& particles);
void createFluid(std::vector<particle>& particles);

void createContainerWithFunnel(std::vector<particle>& particles);
void createFluidAboveFunnel(std::vector<particle>& particles);

void createContainerWithPlatforms(std::vector<particle>& particles);
void createFluidOnPlatforms(std::vector<particle>& particles);

void createContainerWithBarrier(std::vector<particle>& particles);
void createFluidLeftOfBarrier(std::vector<particle>& particles);

std::vector<Scene> createScenes();
