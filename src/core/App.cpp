//
// Created by jack on 5/5/2025.
//

#include "App.h"

#include "Game.h"

App::App()
{

}

App::~App()
{

}

void App::run()
{
    GLFWwindow* window = glfwCreateWindow(1280, 1024, "Yeah somebody gotta do it", nullptr, nullptr);

    Game game;
    game.init();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        while(glfwGet)
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
