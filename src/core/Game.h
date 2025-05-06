//
// Created by jack on 5/5/2025.
//

#ifndef GAME_H
#define GAME_H



class Game {
public:
    void init();
    void input(std::optional<>);
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
};



#endif //GAME_H
