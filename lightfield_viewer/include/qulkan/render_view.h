#pragma once

#ifndef RENDER_VIEW_H
#define RENDER_VIEW_H

#include "imgui.h"
#include "qulkan/handlemanager.h"
#include "qulkan/utils.h"
#include <memory>

namespace Qulkan {

    enum ViewType { OPENGL = 0, VULKAN = 1 };

    enum Antialiasing { NONE = 0, MSAAX2 = 1, MSAAX4 = 2, MSAAX8 = 3 };

    // Make static singleton class maybe
    struct PreferenceManager {
        bool mouseOverlay;
        bool mouseMirror;
        int mirrorWith;
        int mirrorWithCombo;
        int antialiasingCombo;
        PreferenceManager(bool mouseOverlay, int mirrorWith, int mirrorWithCombo, bool mouseMirror, int antialiasingCombo)
            : mouseOverlay(mouseOverlay), mirrorWith(mirrorWith), mirrorWithCombo(mirrorWithCombo), mouseMirror(mouseMirror),
              antialiasingCombo(antialiasingCombo){};
    };

    class RenderView {

      private:
        const char *m_viewName;
        const int m_id;

        bool m_isActive;

        glm::vec2 inRectPos;
        glm::vec2 rectPosMin;
        glm::vec2 rectPosMax;

        bool captureKeyboard;
        bool captureMouse;

        ViewType viewType;
        ImTextureID renderViewTexture;
        unsigned int msRenderFramebuffer;
        unsigned int renderFramebuffer;

        int actualRenderWidth;
        int actualRenderHeight;

      protected:
        // Mouse properties
        glm::vec2 screenMousePos; // normalized inscreen mouse position
        glm::vec2 mouseDelta;
        float mouseWheel;

        int initialRenderWidth;
        int initialRenderHeight;

        HandleManager handleManager;
        PreferenceManager preferenceManager;

        bool initialized;
        bool error;

      public:
        RenderView(const char *viewName = "Render View", int initialRenderWidth = 1920, int initialRenderHeight = 1080, ViewType viewType = ViewType::OPENGL);

        virtual ~RenderView(){};

        /* Inits the render view */
        virtual void init() = 0;

        /* Cleans the render view */
        virtual void clean() = 0;

        /* Returns a texture/rendered image as a ImTextureID pointer for ImGui to render to a renderview */
        virtual void render(int actualRenderWidth, int actualRenderHeight) = 0;

        ImTextureID renderToTexture();

        void recreateFramebuffer(int actualRenderWidth, int actualRenderHeight);

        bool isInitialized() const;

        void recompileShaders();

        HandleManager &getHandleManager();
        PreferenceManager &getPreferenceManager();

        int width() const;
        int height() const;

        unsigned int getRenderFramebuffer() const;
        ImTextureID getRenderViewTexture() const;

        glm::vec2 getInRectPos() const;
        void setInRectPos(glm::vec2 pos);

        glm::vec2 getRectPosMin() const;
        void setRectPosMin(glm::vec2 size);

        glm::vec2 getRectPosMax() const;
        void setRectPosMax(glm::vec2 size);

        void setMousePos(glm::vec2 mousePos);
        glm::vec2 getMousePos() const;

        void setMouseDelta(glm::vec2 _mouseDelta);
        void setMouseWheel(float value);

        void setActive(bool active);
        bool isActive() const;

        const char *name() const;
        const int getId() const;

        void setCaptureKeyboard(bool capture);
        void setCaptureMouse(bool capture);

        bool isKeyDown(int glfw_key) const;

        bool isKeyPressed(int glfw_key) const;

        bool isKeyReleased(int glfw_key) const;

        bool isMouseDragging(int glfw_mouse_button) const;

        bool isMouseDown(int glfw_mouse_button) const;

        bool isMouseClicked(int glfw_mouse_button) const;

        bool isMouseDoubleClicked(int glfw_mouse_button) const;

        bool isMouseReleased(int glfw_mouse_button) const;
    };

} // namespace Qulkan

#endif