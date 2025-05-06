//
// Created by jack on 5/5/2025.
//

#include "Factories.h"

#include "../comp/AnimState.h"
#include "../comp/Direction.h"
#include "../comp/Position.h"
#include "../comp/TriGuy.h"


entt::entity makeTriGuy(entt::registry& reg)
{
    const entt::entity e = reg.create();
    reg.emplace<TriGuy>(e);
    reg.emplace<Direction>(e);
    reg.emplace<Position>(e);
    reg.emplace<AnimState>(e);
}
