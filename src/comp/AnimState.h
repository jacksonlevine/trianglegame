//
// Created by jack on 5/5/2025.
//

#ifndef ANIMSTATE_H
#define ANIMSTATE_H

#include "../PrecompHeader.h"

enum AnimName : uint8_t
{
    DANCE,
    IDLE,
    WALK
};

struct AnimState {
    AnimName animname;
};

#endif //ANIMSTATE_H
