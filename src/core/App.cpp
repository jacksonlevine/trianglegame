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

    GLFWwindow* window = glfwCreateWindow(SWIDTH, SHEIGHT, "TriFarm", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GL\n";
        throw std::runtime_error("No GL");
    }

    glViewport(0, 0, SWIDTH, SHEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.3f, 0.3f, 0.6f, 1.0f);

    Game game;
    game.init();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.logic();
        game.render();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
}
