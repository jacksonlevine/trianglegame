//
// Created by jack on 5/5/2025.
//

#include "App.h"

#include "Game.h"
#include "../PrecompHeader.h"
App::App()
{

}

App::~App()
{
    glfwTerminate();
}


void SC_C(GLFWwindow* window, double xoffset, double yoffset)
{
    auto *game = reinterpret_cast<Game>(glfwGetWindowUserPointer(window));
    game->camera.transform.position.z = std::min(50.0f, std::max(1.0f, game->camera.transform.position.z + (float)yoffset * 0.2f));
}

enum class INPUTS : uint8_t {
    LEFT,
    RIGHT,
    UP,
    DOWN
};
void K_C(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static std::bitset<4> inputs(0);
    if(key == GLFW_KEY_A)
    {
        inputs[(int)INPUTS::LEFT] = action ? 1 : 0;
    }
    if(key == GLFW_KEY_D)
    {
        inputs[(int)INPUTS::RIGHT] = action ? 1 : 0;
    }
    if(key == GLFW_KEY_W)
    {
        inputs[(int)INPUTS::UP] = action ? 1 : 0;
    }
    if(key == GLFW_KEY_S)
    {
        inputs[(int)INPUTS::DOWN] = action ? 1 : 0;
    }


    auto *game = reinterpret_cast<Game>(glfwGetWindowUserPointer(window));


    if(inputs[(int)INPUTS::UP])
    {
        game->camera.transform.position += glm::vec3(0,1,0) * aDeltaTime * 3.0f;
    }
    if(inputs[(int)INPUTS::DOWN])
    {
        game->camera.transform.position += glm::vec3(0,-1,0) * aDeltaTime * 3.0f;
    }
    if(inputs[(int)INPUTS::LEFT])
    {
        game->camera.transform.position += glm::vec3(-1,0,0) * aDeltaTime * 3.0f;
    }
    if(inputs[(int)INPUTS::RIGHT])
    {
        game->camera.transform.position += glm::vec3(1,0,0) * aDeltaTime * 3.0f;
    }


}
void CP_C(GLFWwindow* window, double xpos, double ypos)
{

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

    glfwSetScrollCallback(window, SC_C);
    glfwSetKeyCallback(window, K_C);
    glfwSetCursorPosCallback(window, CP_C);

    glViewport(0, 0, SWIDTH, SHEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.3f, 0.3f, 0.6f, 1.0f);


    Game game;
    game.init();

    glfwSetWindowUserPointer(window, &game);

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
