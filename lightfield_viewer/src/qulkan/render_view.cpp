#include "qulkan/render_view.h"
#include "imgui.h"
#include "qulkan/logger.h"
#include "qulkan/utils.h"
#include <memory>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Qulkan {

    RenderView::RenderView(const char *viewName, int initialRenderWidth, int initialRenderHeight, ViewType viewType)
        : m_id(Qulkan::getNextUniqueID()), m_isActive(false), screenMousePos(glm::vec2(0.5f, 0.5f)), actualRenderWidth(initialRenderWidth),
          actualRenderHeight(initialRenderHeight), initialRenderWidth(initialRenderWidth), initialRenderHeight(initialRenderHeight), m_viewName(viewName),
          initialized(false), error(false), preferenceManager(false, 0, 0, false, 0), viewType(viewType) {

        if (viewType == ViewType::OPENGL) {
            // Initialize with no multisampling
            recreateFramebuffer(initialRenderWidth, initialRenderHeight);

        } else {
            // TODO Vulkan stuff
        }
    }

    void RenderView::recreateFramebuffer(int newRenderWidth, int newRenderHeight) {

        actualRenderWidth = newRenderWidth;
        actualRenderHeight = newRenderHeight;

        int samples = 0;

        switch (this->getPreferenceManager().antialiasingCombo) {
        case Antialiasing::NONE:
            samples = 0;
            break;
        case Antialiasing::MSAAX2:
            samples = 2;
            break;
        case Antialiasing::MSAAX4:
            samples = 4;
            break;
        case Antialiasing::MSAAX8:
            samples = 8;
            break;
        default:
            samples = 0;
            break;
        }

        if (samples != 0) {
            GLuint msTexture;

            glGenTextures(1, &msTexture);

            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msTexture);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, actualRenderWidth, actualRenderWidth, GL_TRUE);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

            // Create framebuffer and attach color multisamples texture
            glGenFramebuffers(1, &msRenderFramebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, msRenderFramebuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msTexture, 0);

            // Create a renderbuffer for depth/stencil operation and attach it to the framebuffer
            GLuint msRbo;
            glGenRenderbuffers(1, &msRbo);
            glBindRenderbuffer(GL_RENDERBUFFER, msRbo);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, actualRenderWidth, actualRenderHeight);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msRbo);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                error = true;
                Qulkan::Logger::Error("FRAMEBUFFER:: Framebuffer Multisampling is not complete!");
            }
        }

        GLuint texture;
        glGenTextures(1, &texture);

        // Render texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, actualRenderWidth, actualRenderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Create framebuffer and attach color texture
        glGenFramebuffers(1, &renderFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        // Create a renderbuffer for depth/stencil operation and attach it to the framebuffer
        GLuint rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, actualRenderWidth, actualRenderHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            error = true;
            Qulkan::Logger::Error("FRAMEBUFFER:: Render Framebuffer is not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        renderViewTexture = (ImTextureID)(intptr_t)texture;
    }

    ImTextureID RenderView::renderToTexture() {

        if (this->getPreferenceManager().antialiasingCombo != 0) { // Antialiasing on

            glBindFramebuffer(GL_FRAMEBUFFER, msRenderFramebuffer);

            render(actualRenderWidth, actualRenderHeight);

            glBindFramebuffer(GL_READ_FRAMEBUFFER, msRenderFramebuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderFramebuffer);
            glBlitFramebuffer(0, 0, actualRenderWidth, actualRenderHeight, 0, 0, actualRenderWidth, actualRenderHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        } else { // Antialiasing off

            glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);

            render(actualRenderWidth, actualRenderHeight);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        return renderViewTexture;
    };

    bool RenderView::isInitialized() const { return initialized; }

    void RenderView::recompileShaders() {
        Qulkan::Logger::Info("%s: Recompiling Shader \n", m_viewName);
        initialized = false;
        clean();
        init();
        recreateFramebuffer(actualRenderWidth, actualRenderHeight);
    }

    HandleManager &RenderView::getHandleManager() { return handleManager; }
    PreferenceManager &RenderView::getPreferenceManager() { return preferenceManager; };

    int RenderView::width() const { return actualRenderWidth; }
    int RenderView::height() const { return actualRenderHeight; }

    unsigned int RenderView::getRenderFramebuffer() const { return renderFramebuffer; }
    ImTextureID RenderView::getRenderViewTexture() const { return renderViewTexture; }

    glm::vec2 RenderView::getInRectPos() const { return inRectPos; }
    void RenderView::setInRectPos(glm::vec2 pos) { inRectPos = pos; }

    glm::vec2 RenderView::getRectPosMin() const { return rectPosMin; }
    void RenderView::setRectPosMin(glm::vec2 size) { rectPosMin = size; }

    glm::vec2 RenderView::getRectPosMax() const { return rectPosMax; }
    void RenderView::setRectPosMax(glm::vec2 size) { rectPosMax = size; }

    void RenderView::setMousePos(glm::vec2 mousePos) { screenMousePos = mousePos; }
    glm::vec2 RenderView::getMousePos() const { return screenMousePos; }

    void RenderView::setMouseDelta(glm::vec2 _mouseDelta) { mouseDelta = _mouseDelta; }
    void RenderView::setMouseWheel(float value) { mouseWheel = value; }

    void RenderView::setActive(bool active) { m_isActive = active; }
    bool RenderView::isActive() const { return m_isActive; }

    const char *RenderView::name() const { return m_viewName; }
    const int RenderView::getId() const { return m_id; }

    void RenderView::setCaptureKeyboard(bool capture) { captureKeyboard = capture; }
    void RenderView::setCaptureMouse(bool capture) { captureMouse = capture; }

    bool RenderView::isKeyDown(int glfw_key) const {
        if (captureKeyboard) {
            return ImGui::IsKeyDown(glfw_key);
        } else {
            return false;
        }
    }

    bool RenderView::isKeyPressed(int glfw_key) const {
        if (captureKeyboard) {
            return ImGui::IsKeyPressed(glfw_key);
        } else {
            return false;
        }
    }

    bool RenderView::isKeyReleased(int glfw_key) const {
        if (captureKeyboard) {
            return ImGui::IsKeyReleased(glfw_key);
        } else {
            return false;
        }
    }

    bool RenderView::isMouseDragging(int glfw_mouse_button) const {
        if (captureMouse) {
            return ImGui::IsMouseDragging(glfw_mouse_button);
        } else {
            return false;
        }
    }

    bool RenderView::isMouseDown(int glfw_mouse_button) const {
        if (captureMouse) {
            return ImGui::GetIO().MouseDownDuration[glfw_mouse_button] >= 0.0f;
        } else {
            return false;
        }
    }

    bool RenderView::isMouseClicked(int glfw_mouse_button) const {
        if (captureMouse) {
            return ImGui::IsMouseClicked(glfw_mouse_button);
        } else {
            return false;
        }
    }

    bool RenderView::isMouseDoubleClicked(int glfw_mouse_button) const {
        if (captureMouse) {
            return ImGui::IsMouseDoubleClicked(glfw_mouse_button);
        } else {
            return false;
        }
    }
    bool RenderView::isMouseReleased(int glfw_mouse_button) const {
        if (captureMouse) {
            return ImGui::IsMouseReleased(glfw_mouse_button);
        } else {
            return false;
        }
    }

} // namespace Qulkan
