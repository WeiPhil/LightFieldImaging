#include "qulkan/utils.h"

namespace Qulkan {

    int uniqueID = 0;
    u_int64_t frameNumber = 0;
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    int getNextUniqueID() { return ++uniqueID; }
    void updateFrameNumber() { ++frameNumber; }
    void updateDeltaTime(float currentFrameTime) {
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
    }
    u_int64_t getFrameNumber() { return frameNumber; }
    float getDeltaTime() { return deltaTime; }

} // namespace Qulkan
