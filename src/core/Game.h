//
// Created by jack on 5/5/2025.
//

#ifndef GAME_H
#define GAME_H
#include "../Camera.h"


class Game {
public:
    void init();
    void input(inputsType& inputs); //glfw key
    bool logic();
    void render();

    jl::Camera camera;
private:
    enum class State {
        playing,
        won,
        lost
      };

    std::mt19937 rand;
    State state = State::playing;
    entt::registry reg;

};



#endif //GAME_H
