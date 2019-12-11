#pragma once

#ifndef FRAMEBUFFERMANAGER_H
#define FRAMEBUFFERMANAGER_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <vector>

#include "qulkan/utils.h"

class FramebufferManager {

  private:
    int m_max;
    std::map<std::string, int> m_framebuffers_map;

  public:
    FramebufferManager() : m_max(0){};
    ~FramebufferManager(){};

    std::vector<GLuint> framebuffers;

    int size() const { return m_max; }

    void addFramebuffer(std::string framebufferName) {
        m_framebuffers_map[framebufferName] = m_max;
        ++m_max;
        framebuffers.resize(m_max);
    }

    GLuint operator()(const std::string framebufferName) { return framebufferID(framebufferName); }

    GLuint framebufferID(const std::string framebufferName) {
        ASSERT(m_framebuffers_map.find(framebufferName) != m_framebuffers_map.end(), "No framebuffer with that name found");

        return framebuffers[m_framebuffers_map[framebufferName]];
    }
};

#endif
