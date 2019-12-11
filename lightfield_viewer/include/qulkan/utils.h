#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <string>

#include <iostream>

#define ASSERT(boolean, message)                                                                                                                               \
    {                                                                                                                                                          \
        if (boolean != true) {                                                                                                                                 \
            std::cout << "[ASSERT FAILED] " << message << std::endl;                                                                                           \
            assert(false);                                                                                                                                     \
        }                                                                                                                                                      \
    }

#define C_STRING(value) std::to_string(value).c_str()

namespace Qulkan {

    extern int getNextUniqueID();
    extern void updateFrameNumber();
    extern void updateDeltaTime(float currentFrameTime);
    extern u_int64_t getFrameNumber();
    extern float getDeltaTime();
} // namespace Qulkan
#endif