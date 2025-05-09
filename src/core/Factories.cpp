//
// Created by jack on 5/5/2025.
//

#include "Factories.h"

#include "../Camera.h"
#include "../comp/AnimState.h"
#include "../comp/Behaviors.h"
#include "../comp/Direction.h"
#include "../comp/MusicMachine.h"
#include "../comp/Position.h"
#include "../comp/TriGuy.h"
#include "../util/DirHelpers.h"


entt::entity makeTriGuy(entt::registry& reg, const glm::vec2& pos, float heading, std::mt19937& rng)
{
    const entt::entity e = reg.create();
    static std::uniform_real_distribution<float> distrib(3.0f, 12.0f);
    reg.emplace<TriGuy>(e, distrib(rng));
    reg.emplace<Direction>(e, heading, headingToDirection(heading));
    reg.emplace<Position>(e, pos);
    reg.emplace<AnimState>(e, IDLE);
    reg.emplace<Wandering>(e, 0.0f);
    return e;
}


entt::entity makeMusicMachine(entt::registry& reg, const glm::ivec2& pos)
{
    const entt::entity e = reg.create();
    reg.emplace<MusicMachine>(e);
    reg.emplace<AnimState>(e, MM_DANCE);
    reg.emplace<Position>(e, glm::vec2(pos.x, pos.y));
    return e;
}

