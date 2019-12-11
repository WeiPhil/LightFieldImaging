
#include "qulkan/windows.h"

#include "imgui.h"
#include "qulkan/handlemanager.h"
#include "qulkan/logger.h"
#include "qulkan/utils.h"

#include "utils/pngwriter.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <map>
#include <memory>

namespace Qulkan {

    /*! \brief Create a Docking Space
     *         The docking space needs to be rendered before any other window.
     *
     *
     *  For more information about the docking space refer to imgui_demo.cpp
     *
     */
    void dockingSpace() {
        bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
        // not dockable into, because it would be confusing to have two docking
        // targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
        // our background and handle the pass-thru hole, so we ask Begin() to not
        // render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Test", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO &io = ImGui::GetIO();
        // DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("Create New Project");
                ImGui::Separator();
                ImGui::MenuItem("Exit");

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                ImGui::MenuItem("Preferences...");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::MenuItem("About Qulkan...");
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End(); // End DockSpace Test

        return;
    }

    /*! \brief Helper for creating the handles in the configuration view
     *
     *
     */
    void mirrorHandles(const HandleManager &srcHandler, const HandleManager &destHandler) {
        for (auto const &srcHandle : srcHandler.getHandles()) {
            for (auto const &destHandle : destHandler.getHandles()) {
                if (srcHandle->name == destHandle->name) {
                    destHandle->value = srcHandle->value;
                }
            }
        }
    }

    /*! \brief Helper for creating the handles in the configuration view
     *
     *
     */
    void handleParser(const HandleManager &handleManager) {
        for (auto const &handle : handleManager.getHandles()) {
            if (handle->isActive()) {
                bool hasActivator = handle->active != nullptr;
                if (hasActivator)
                    ImGui::Indent();

                switch (handle->type) {
                case Type::BOOL: {
                    ImGui::Checkbox(handle->name.c_str(), std::any_cast<bool>(&handle->value));
                    handle->invValue = !std::any_cast<bool>(handle->value);
                    break;
                }
                case Type::INT: {
                    ImGui::SliderInt(handle->name.c_str(), std::any_cast<int>(&handle->value), handle->getMinValues<int>(), handle->getMaxValues<int>(), "%d");
                    break;
                }
                case Type::FLOAT: {

                    ImGui::SliderFloat(handle->name.c_str(), std::any_cast<float>(&handle->value), handle->getMinValues<float>(), handle->getMaxValues<float>(),
                                       "%.4f");
                    break;
                }
                case Type::RADIAN_FLOAT: {

                    ImGui::SliderAngle(handle->name.c_str(), std::any_cast<float>(&handle->value), handle->getMinValues<float>() * 180.0f / M_PI,
                                       handle->getMaxValues<float>() * 180.0f / M_PI, "%.2f°");
                    break;
                }
                case Type::VEC2: {
                    auto val = std::any_cast<glm::vec2>(&handle->value);
                    auto min = handle->getMinValues<glm::vec2>().x;
                    auto max = handle->getMaxValues<glm::vec2>().x;
                    ImGui::SliderFloat2(handle->name.c_str(), (float *)val, min, max, "%.4f");
                    break;
                }
                case Type::VEC3: {
                    auto val = std::any_cast<glm::vec3>(&handle->value);
                    auto min = handle->getMinValues<glm::vec3>().x;
                    auto max = handle->getMaxValues<glm::vec3>().x;
                    ImGui::SliderFloat3(handle->name.c_str(), (float *)val, min, max, "%.4f");
                    break;
                }
                case Type::COLOR3: {
                    auto val = std::any_cast<glm::vec3>(&handle->value);
                    ImGui::ColorEdit3(handle->name.c_str(), (float *)val);
                    break;
                }
                case Type::TEXT: {
                    ImGui::TextWrapped("%s", handle->getValue<const char *>());

                    break;
                }
                default:
                    ImGui::TextColored(ImVec4(0.8, 0.2, 0.2, 1.0), " %s: No implementation for this type of handler (%s)\n", handle->name.c_str(),
                                       toString(handle->type));
                    break;
                }
                if (hasActivator)
                    ImGui::Unindent();
            }
        }
        if (handleManager.getHandles().size() == 0) {

            ImGui::TextWrapped("You haven't defined any handles for this view now.\n");
            ImGui::Spacing();
            ImGui::TextWrapped("You can do so by using in your implementation (handleManager herited from RenderView class):\n\n");
            ImGui::Indent();
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Handle yourHandle(\"handleName\", Type::SUPPORTED_TYPE, initialValue);\n"
                                                               "handleManager.addHandle(yourHandle);\n\n");
            ImGui::PopFont();

            ImGui::Unindent();
        }
    }

    /* Inits all render views */
    void initViews(std::vector<std::reference_wrapper<RenderView>> &renderViews) {
        for (RenderView &renderView : renderViews)
            renderView.init();
    }

    /*! \brief Render the windows for the main view
     *
     *
     */
    void renderWindows(std::vector<std::reference_wrapper<RenderView>> &renderViews) {
        ImGuiIO &io = ImGui::GetIO();

        for (RenderView &renderView : renderViews) {

            if (!renderView.isInitialized())
                continue;
            // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));

            // Main window containing the OpenGL/Vulkan rendering
            ImGui::SetNextWindowSize(ImVec2(renderView.width(), renderView.height()), ImGuiCond_FirstUseEver);

            ImGui::Begin(renderView.name(), nullptr, ImGuiWindowFlags_NoNavInputs & ImGuiWindowFlags_NoScrollWithMouse & ImGuiWindowFlags_NoMouseInputs);

            ImGui::PushID(renderView.getId());

            // Setting view ratio
            float ratio = (float)renderView.width() / (float)renderView.height();

            bool keepTextureRatio = true;

            float w = ImGui::GetContentRegionAvail().x;
            float h = ImGui::GetContentRegionAvail().y;

            glm::vec2 screenPos = ImGui::GetCursorScreenPos();
            glm::vec2 endPos = glm::vec2(screenPos.x + w, screenPos.y + h);
            glm::vec2 endPosNoRatio = endPos;

            ImTextureID tex = renderView.renderToTexture();

            float space = 0.0f;

            // Render texture to window
            if (!keepTextureRatio)
                ImGui::GetWindowDrawList()->AddImage(tex, screenPos, endPos);
            else {
                float minSize = glm::min(w, h * ratio);
                float maxSize = glm::max(w, h * ratio);

                bool widthIsMax = w > h * ratio;

                space = widthIsMax ? (maxSize - minSize) * 0.5f : (maxSize - minSize) * 0.5f / ratio;

                if (widthIsMax) {
                    screenPos.x += space;
                    endPos.x -= space;
                } else {
                    screenPos.y += space;
                    endPos.y -= space;
                }

                ImGui::GetWindowDrawList()->AddImage(tex, screenPos, endPos, ImVec2(0, 1), ImVec2(1, 0));
            }

            // Setting button overlay position
            glm::vec2 currentViewportSize = (renderView.getRectPosMax() - renderView.getRectPosMin());
            bool fbResizeActive = currentViewportSize != glm::vec2(renderView.width(), renderView.height());
            if (fbResizeActive) {

                if (ImGui::Button("Reset Resolution")) {
                    renderView.recreateFramebuffer(currentViewportSize.x, currentViewportSize.y);
                }
                ImGui::Text("Framebuffer Resolution: %d x %d \nCurrent Resolution %0.f x %0.f", renderView.width(), renderView.height(), currentViewportSize.x,
                            currentViewportSize.y);
            }

            glm::vec2 screenMousePos = glm::vec2(io.MousePos.x - screenPos.x, io.MousePos.y - screenPos.y);
            glm::vec2 diff = (endPosNoRatio - endPos) * 2.0f;
            glm::vec2 textureEndPos = glm::vec2(w, h) - diff;

            // Sets the window size actually seen in the application
            renderView.setInRectPos(screenMousePos);
            renderView.setRectPosMin(screenPos);
            renderView.setRectPosMax(screenPos + textureEndPos);
            renderView.setActive(ImGui::IsWindowFocused());

            ImGui::PopID();

            ImGui::End(); // end renderview

            if (renderView.getPreferenceManager().mouseOverlay && renderView.isActive()) {
                ImGui::SetNextWindowPos(renderView.getRectPosMin() - glm::vec2(0.0, space) + glm::vec2(10, 10));
                ImGui::SetNextWindowBgAlpha(0.35f);
                if (ImGui::Begin("Mouse Overlay", &renderView.getPreferenceManager().mouseOverlay,
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
                    ImGui::Text("Mouse View Position: (%.1f,%.1f)", renderView.getInRectPos().x, renderView.getInRectPos().y);
                    auto normPos = renderView.getInRectPos() / (renderView.getRectPosMax() - renderView.getRectPosMin());
                    ImGui::Text("Normalized View Position: (%.1f,%.1f)", normPos.x, normPos.y);
                }
                ImGui::End();
            }
        }

        // ImGui::PopStyleColor();
    } // namespace Qulkan

    void viewConfigurations(std::vector<std::reference_wrapper<RenderView>> &renderViews) {

        // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));

        // Main window containing the OpenGL/Vulkan rendering
        ImGui::SetNextWindowSize(ImVec2(300, 800), ImGuiCond_FirstUseEver);

        ImGui::Begin("View Configurations");

        //       src, dest
        std::map<int, int> newMousePos;
        std::map<int, int> copyHandles;

        int currIdx = 0;
        for (RenderView &renderView : renderViews) {
            if (renderView.getPreferenceManager().mirrorWithCombo != 0) {
                if (copyHandles.find(currIdx) == copyHandles.end()) {
                    copyHandles[renderView.getPreferenceManager().mirrorWith] = currIdx;
                }

                if (renderView.getPreferenceManager().mouseMirror && newMousePos.find(currIdx) == newMousePos.end()) {
                    newMousePos[renderView.getPreferenceManager().mirrorWith] = currIdx;
                }
            }
            ++currIdx;
        }

        currIdx = 0;
        for (RenderView &renderView : renderViews) {
            // Update Mouse Pos
            if (newMousePos.find(currIdx) != newMousePos.end()) {
                renderViews[newMousePos[currIdx]].get().setMousePos(renderViews[currIdx].get().getMousePos());
            }

            // View Creation
            ImGui::PushID(currIdx);
            if (ImGui::CollapsingHeader(renderView.name())) {

                std::vector<const char *> renderViewNames = {"None"};
                for (RenderView &_renderView : renderViews) {
                    if (std::string(_renderView.name()) != renderView.name())
                        renderViewNames.push_back(_renderView.name());
                }

                if (ImGui::TreeNode("Shaders")) {
                    if (ImGui::Button("Recompile Shader")) {
                        renderView.recompileShaders();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("View")) {
                    ImGui::Spacing();
                    static char filename[512] = "filename.png";
                    ImGui::PushItemWidth(-140);
                    ImGui::InputText("Filename Path", filename, IM_ARRAYSIZE(filename));
                    if (ImGui::Button("Save Framebuffer")) {
                        float *pixels = new float[renderView.width() * renderView.height() * 4];
                        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)renderView.getRenderViewTexture());
                        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);
                        glBindTexture(GL_TEXTURE_2D, 0);
                        PNGWriter pngWriter(renderView.width(), renderView.height(), 4, pixels);
                        const char *imageRefTitle = "framebufferImage";
                        pngWriter.writePNG(filename, imageRefTitle);
                    }
                    ImGui::PopItemWidth();
                    // ImGui::Indent();
                    // ImGui::Separator();
                    // ImGui::Unindent();
                    ImGui::Spacing();

                    std::vector<const char *> antialiasingMethods = {"None", "2x msaa", "4x msaa", "8x msaa"};
                    if (ImGui::Combo("Antialiasing", &renderView.getPreferenceManager().antialiasingCombo, &antialiasingMethods[0], //
                                     antialiasingMethods.size(), 8)) {
                        renderView.recreateFramebuffer(renderView.height(), renderView.width());
                    }

                    ImGui::Checkbox("Mouse position overlay", &renderView.getPreferenceManager().mouseOverlay);

                    ImGui::PushItemWidth(-140);

                    ImGui::Combo("Mirror handles", &renderView.getPreferenceManager().mirrorWithCombo, &renderViewNames[0], renderViewNames.size(), 4);
                    // Correct index
                    if (renderView.getPreferenceManager().mirrorWithCombo != 0) {
                        renderView.getPreferenceManager().mirrorWith = renderView.getPreferenceManager().mirrorWithCombo <= currIdx
                                                                           ? renderView.getPreferenceManager().mirrorWithCombo - 1
                                                                           : renderView.getPreferenceManager().mirrorWithCombo;

                        ImGui::Indent();
                        ImGui::Checkbox("Mirror mouse", &renderView.getPreferenceManager().mouseMirror);
                        ImGui::Unindent();
                    }
                    ImGui::PopItemWidth();
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Handles")) {
                    ImGui::Spacing();
                    if (copyHandles.find(currIdx) != copyHandles.end()) {
                        // ImGui::Text("%s to %s , vec(%.1f,%.1f)", renderViews[currIdx]->name(), renderViews[copyHandles[currIdx]]->name(), temp.x, temp.y);

                        mirrorHandles(renderView.getHandleManager(), renderViews[copyHandles[currIdx]].get().getHandleManager());
                    }
                    auto handleManager = renderView.getHandleManager();
                    handleParser(handleManager);
                    ImGui::TreePop();
                }
            }

            ImGui::PopID();
            currIdx++;
        }

        ImGui::End();
    }

} // namespace Qulkan