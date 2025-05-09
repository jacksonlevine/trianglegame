//
// Created by jack on 5/5/2025.
//

#ifndef RENDER_H
#define RENDER_H

#include "../PrecompHeader.h"
#include "../Camera.h"

void renderBackground(entt::registry &reg, const jl::Camera& cam);

void renderTriGuys(entt::registry &reg, const jl::Camera& cam);

void renderMusicMachines(entt::registry &reg, const jl::Camera& cam);

#endif //RENDER_H
