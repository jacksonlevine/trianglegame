//
// Created by jack on 5/6/2025.
//

#ifndef DIRHELPERS_H
#define DIRHELPERS_H

#include "../PrecompHeader.h"

inline glm::vec2 headingToDirection(float heading) {
    heading -= std::floor(heading);
    float angle = (0.5f - heading) * 2.0f * glm::pi<float>();
    return glm::vec2(std::sin(angle), std::cos(angle));
}

inline float directionToHeading(glm::vec2 direction) {
    if (glm::length(direction) <= 0.0f) return 0.0f;
    direction = glm::normalize(direction);
    float angle = std::atan2(direction.x, direction.y);
    float heading = (0.5f - angle / (2.0f * glm::pi<float>()));
    return heading - std::floor(heading);
}
#endif //DIRHELPERS_H
