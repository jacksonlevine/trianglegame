//
// Created by jack on 5/6/2025.
//

#include "TriGuyBehaviors.h"

#include "../comp/AnimState.h"
#include "../comp/Behaviors.h"
#include "../comp/Direction.h"
#include "../comp/MusicMachine.h"
#include "../comp/Position.h"
#include "../comp/TriGuy.h"
#include "../util/DirHelpers.h"


void doTriBehaviors(entt::registry& registry, float deltaTime, std::mt19937& rng)
{
    {
        //Wander the wanderers
        const auto view = registry.view<TriGuy, Wandering, Position>();
        auto dist = std::uniform_real_distribution<float>(1.0f, 15.0f);
        auto movedist = std::uniform_real_distribution<float>(-5.0f, 5.0f);

        for (auto entity : view)
        {
            auto& wandering = view.get<Wandering>(entity);
            auto& position = view.get<Position>(entity);
            if (wandering.changeInterval >= dist(rng))
            {
                if (!registry.all_of<FocusPoint>(entity))
                {
                    registry.emplace<FocusPoint>(entity, position.position + glm::vec2(movedist(rng), movedist(rng)));
                } else
                {
                    registry.patch<FocusPoint>(entity, [&](auto & p)
                    {
                        p.point += glm::vec2(movedist(rng), movedist(rng));
                    });
                }

                if (!registry.all_of<WalkMode>(entity))
                {
                    registry.emplace<WalkMode>(entity);
                }

                wandering.changeInterval = 0.0f;
            } else
            {
                wandering.changeInterval += deltaTime;
            }
        }
    }

    //Walk the walkers
    {
        const auto view = registry.view<TriGuy, WalkMode, Position, AnimState, FocusPoint, Direction>();
        for (auto entity : view)
        {
            auto & focusPoint = view.get<FocusPoint>(entity);
            auto & position = view.get<Position>(entity);
            auto & animState = view.get<AnimState>(entity);
            auto & direction = view.get<Direction>(entity);
            if (glm::distance(focusPoint.point, position.position) < 0.5f)
            {
                animState.animname = IDLE;
            } else
            {
                auto newdir = glm::normalize(focusPoint.point - position.position);
                animState.animname = WALK;
                direction.direction = newdir;
                direction.heading = directionToHeading(newdir);
                position.position = position.position + (newdir*deltaTime*2.0f);
            }
        }
    }

    //Notice active music machines
    {
        const auto guyview = registry.view<TriGuy, Position>();
        const auto machineview = registry.view<MusicMachine, AnimState, Position>();
        for (auto entity : guyview)
        {
            auto& [position] = registry.get<Position>(entity);

            bool foundmachine = false;
            for (auto mmentity : machineview)
            {
                auto& mmanimState = registry.get<AnimState>(mmentity);
                if (mmanimState.animname == MM_DANCE)
                {
                    auto& [mmposition] = registry.get<Position>(mmentity);
                    if (distance(mmposition, position) < 125.0f)
                    {
                        foundmachine = true;
                        if (!registry.all_of<FocusPoint>(entity))
                        {
                            registry.emplace<FocusPoint>(entity, mmposition);
                        }
                        if (!registry.all_of<DanceMode>(entity))
                        {
                            registry.emplace<DanceMode>(entity);
                        }

                        if (registry.all_of<WalkMode>(entity))
                        {
                            registry.remove<WalkMode>(entity);
                        }
                        if (registry.all_of<Wandering>(entity))
                        {
                            registry.remove<Wandering>(entity);
                        }
                        break;
                    }
                }

            }

            if (!foundmachine)
            {
                if (!registry.all_of<Wandering>(entity))
                {
                    registry.emplace<Wandering>(entity);
                }
            }

        }
    }

    //Dance the dancers
    {
        const auto view = registry.view<TriGuy, DanceMode, Position, AnimState, FocusPoint, Direction>();
        for (auto entity : view)
        {
            auto & focusPoint = view.get<FocusPoint>(entity);
            auto & position = view.get<Position>(entity);
            auto & animState = view.get<AnimState>(entity);
            auto & direction = view.get<Direction>(entity);
            auto & triguy = view.get<TriGuy>(entity);

            if (glm::distance(focusPoint.point, position.position) < triguy.acceptableObservingDistance)
            {
                animState.animname = DANCE;
            } else
            {
                auto newdir = glm::normalize(focusPoint.point - position.position);
                animState.animname = WALK;
                direction.direction = newdir;
                direction.heading = directionToHeading(newdir);
                position.position = position.position + (newdir*deltaTime*2.0f);
            }
        }
    }
}
