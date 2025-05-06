//
// Created by jack on 5/5/2025.
//

#include "Game.h"

#include "Factories.h"
#include "../sys/Render.h"

void Game::init()
{
    rand.seed(std::random_device{}());
    makeTriGuy(reg);
}

void Game::input(std::optional<int> input)
{
}

bool Game::logic()
{
}

void Game::render()
{
    renderTriGuys(reg);
}
