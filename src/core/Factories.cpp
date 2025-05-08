//
// Created by jack on 5/5/2025.
//

#include "Factories.h"

#include "../Camera.h"
#include "../comp/AnimState.h"
#include "../comp/Behaviors.h"
#include "../comp/Direction.h"
#include "../comp/Position.h"
#include "../comp/TriGuy.h"
#include "../util/DirHelpers.h"


entt::entity makeTriGuy(entt::registry& reg, const glm::vec2& pos, float heading)
{
    const entt::entity e = reg.create();
    reg.emplace<TriGuy>(e);
    reg.emplace<Direction>(e, heading, headingToDirection(heading));
    reg.emplace<Position>(e, pos);
    reg.emplace<AnimState>(e, IDLE);
    reg.emplace<Wandering>(e, 0.0f);
    return e;
}
