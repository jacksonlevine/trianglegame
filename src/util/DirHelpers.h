//
// Created by jack on 5/6/2025.
//

#ifndef DIRHELPERS_H
#define DIRHELPERS_H

#include "../PrecompHeader.h"

// Convert normalized heading (0-1) to direction vector
inline glm::vec2 headingToDirection(float heading) {
    heading -= std::floor(heading); // Wrap to [0,1)
    float angle = heading * 2.0f * glm::pi<float>();
    return glm::vec2(std::sin(angle), -std::cos(angle));
}

// Convert direction vector to normalized heading (0-1)
inline float directionToHeading(glm::vec2 direction) {
    if (glm::length(direction) <= 0.0f) {
        return 0.0f;
    }

    direction = glm::normalize(direction);
    float angle = std::atan2(direction.y, direction.x);
    angle = glm::pi<float>() * 0.5f - angle;

    float heading = angle / (2.0f * glm::pi<float>());
    return heading < 0.0f ? heading + 1.0f : heading;
}

#endif //DIRHELPERS_H
