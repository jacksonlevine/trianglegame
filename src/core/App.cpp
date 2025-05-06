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
    glfwTerminate();
}

void App::run()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1280, 1024, "Yeah somebody gotta do it", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GL\n";
        throw std::runtime_error("No GL");
    }

    Game game;
    game.init();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        game.logic();
        game.render();
    }
    glfwDestroyWindow(window);
}
