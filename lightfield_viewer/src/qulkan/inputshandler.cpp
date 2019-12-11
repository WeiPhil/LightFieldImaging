

#include "qulkan/inputshandler.h"

#include <glm/glm.hpp>

namespace Qulkan {

    void handleInputs(std::vector<std::reference_wrapper<RenderView>> &renderViews) {
        ImGuiIO &io = ImGui::GetIO();

        for (RenderView &renderView : renderViews) {
            if (ImGui::IsMouseHoveringRect(renderView.getRectPosMin(), renderView.getRectPosMax(), false) && renderView.isActive()) {
                renderView.setCaptureMouse(true);
                glm::vec2 renderRectSize = renderView.getRectPosMax() - renderView.getRectPosMin();

                renderView.setMousePos(renderView.getInRectPos() / renderRectSize); // set normalized coordinates
                renderView.setMouseDelta(ImGui::GetMouseDragDelta());
                renderView.setMouseWheel(io.MouseWheel);
            } else {
                renderView.setCaptureMouse(false);
            }
            if (renderView.isActive()) {
                renderView.setCaptureKeyboard(true);
            } else {
                renderView.setCaptureKeyboard(false);
            }
        }
    }
} // namespace Qulkan