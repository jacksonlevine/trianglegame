//
// Created by jack on 5/5/2025.
//

#include "Game.h"

#include "Factories.h"
#include "../sys/Render.h"
#include "../sys/TriGuyBehaviors.h"

void Game::init()
{
    rand.seed(std::random_device{}());

    std::uniform_real_distribution<float> distrib(-50.0f, 50.f);
    std::uniform_real_distribution<float> hdistrib(0.0f, 0.9999f);
    for (int i = 0; i < 100; i++)
    {
        float x = distrib(rand);
        float y = distrib(rand);
        float heading = hdistrib(rand);

        makeTriGuy(reg, glm::vec2(x,y), heading);
    }

    //initialize 3d camera facing game surface (surface spans XY, toward the camera Z, away -Z)
    camera.transform.position = glm::vec3(0, 0, 20);
    camera.updateProjection(SWIDTH, SHEIGHT, 90.0f);
    camera.updateWithYawPitch(-90.0f, 0.f);
}

void Game::input(std::optional<int> input)
{

}

bool Game::logic()
{
    return true;
}

void Game::render()
{
    static auto currenttime = glfwGetTime();
    auto deltaTime = glfwGetTime() - currenttime;
    currenttime = glfwGetTime();

    aDeltaTime = deltaTime;

    //update the cameras view with its current position etc
    camera.updateWithYawPitch(camera.transform.yaw, camera.transform.pitch);
    
    renderBackground(reg, camera);
    doTriBehaviors(reg, deltaTime, rand);
    renderTriGuys(reg, camera);
}
