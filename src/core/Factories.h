//
// Created by jack on 5/5/2025.
//

#ifndef FACTORIES_H
#define FACTORIES_H

#include "../PrecompHeader.h"


entt::entity makeTriGuy(entt::registry& reg, const glm::vec2& pos, float heading, std::mt19937& rng);

entt::entity makeMusicMachine(entt::registry& reg, const glm::ivec2& pos);

#endif //FACTORIES_H
