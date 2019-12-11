#pragma once

#ifndef WINDOWS_H
#define WINDOWS_H

#include "render_view.h"
#include <functional>

namespace Qulkan {

    /* Constructs the docking space */
    void dockingSpace();

    /* Inits all render views */
    void initViews(std::vector<std::reference_wrapper<RenderView>> &renderViews);

    /* Constructs the rendering windows */
    void renderWindows(std::vector<std::reference_wrapper<RenderView>> &renderViews);

    /* Constructs the panel for the handlers of each view*/
    void viewConfigurations(std::vector<std::reference_wrapper<RenderView>> &renderViews);

} // namespace Qulkan

#endif