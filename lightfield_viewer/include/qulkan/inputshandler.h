#pragma once

#ifndef INPUTSHANDLER_H
#define INPUTSHANDLER_H

#include "imgui.h"
#include "qulkan/render_view.h"

#include <functional>
#include <vector>

namespace Qulkan {

    void handleInputs(std::vector<std::reference_wrapper<RenderView>> &renderViews);

} // namespace Qulkan

#endif
