//
// Created by jack on 5/5/2025.
//

#ifndef GAME_H
#define GAME_H
#include "../Camera.h"


class Game {
public:
    void init();
    void input(std::optional<int> input); //glfw key
    bool logic();
    void render();

private:
    enum class State {
        playing,
        won,
        lost
      };

    std::mt19937 rand;
    State state = State::playing;
    entt::registry reg;
    jl::Camera camera;
};



#endif //GAME_H
