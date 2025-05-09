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


GLFWcursor* loadGrabbingCursor(const char* filename, int hotspotX, int hotspotY) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); // force RGBA

    if (!data) {
        fprintf(stderr, "Failed to load cursor image: %s\n", filename);
        return nullptr;
    }

    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = data;

    GLFWcursor* cursor = glfwCreateCursor(&image, hotspotX, hotspotY);
    stbi_image_free(data);
    return cursor;
}




void SC_C(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
    game->camera.transform.position.z = std::min(50.0f, std::max(1.0f, game->camera.transform.position.z - (float)yoffset));
}


static inputsType inputs(0);
void K_C(GLFWwindow* window, int key, int scancode, int action, int mods)
{


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





}

static int cursorposlastx = 0, cursorposlasty = 0;
void MB_C(GLFWwindow* window, int button, int action, int mods)
{
        if (button == GLFW_MOUSE_BUTTON_3)
        {
            if (action == GLFW_PRESS)
            {
                glfwSetCursor(window, grabCursor);
                inputs[(int)INPUTS::FIRSTMIDB] = 1;
                inputs[(int)INPUTS::MIDBHELD] = 1;
            }
            else if (action == GLFW_RELEASE)
            {
                glfwSetCursor(window, nullptr);
                inputs[(int)INPUTS::MIDBHELD] = 0;
                inputs[(int)INPUTS::FIRSTMIDB] = 1;
            }
        }
}
void CP_C(GLFWwindow* window, double xpos, double ypos)
{


    if (inputs[(int)INPUTS::MIDBHELD])
    {

        if (inputs[(int)INPUTS::FIRSTMIDB])
        {
            inputs[(int)INPUTS::FIRSTMIDB] = 0;
            cursorposlastx = xpos;
            cursorposlasty = ypos;
        }

        auto* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
        auto xoffset = xpos - cursorposlastx;
        auto yoffset = ypos - cursorposlasty;

        cursorposlastx = xpos;
        cursorposlasty = ypos;

        game->camera.transform.position.x += -(xoffset / SWIDTH) * 15.0f;
        game->camera.transform.position.y += (yoffset / SHEIGHT) * 15.0f;
    }
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
    glfwSetMouseButtonCallback(window, MB_C);

    glViewport(0, 0, SWIDTH, SHEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.3f, 0.3f, 0.6f, 1.0f);

    grabCursor = loadGrabbingCursor("resourcestoembed/grabcursor.png", 27, 25);
    Game game;
    game.init();

    glfwSetWindowUserPointer(window, &game);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        game.input(inputs);
        game.logic();
        game.render();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
}
