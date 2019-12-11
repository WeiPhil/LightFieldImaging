#pragma once

#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <vector>

#include "qulkan/utils.h"

class BufferManager {

  private:
    int m_max;
    std::map<std::string, int> m_buffers_map;

  public:
    BufferManager() : m_max(0){};
    ~BufferManager(){};

    std::vector<GLuint> buffers;

    int size() const { return m_max; }

    void addBuffer(std::string bufferName) {
        m_buffers_map[bufferName] = m_max;
        ++m_max;
        buffers.resize(m_max);
        // for (auto const &a : buffers) {
        //     std::cout << m_max << std::endl;
        //     std::cout << "buff " << m_buffers_map[bufferName] << std::endl;
        // }
    }

    GLuint operator()(const std::string bufferName) { return bufferID(bufferName); }

    GLuint bufferID(const std::string bufferName) {
        ASSERT(m_buffers_map.find(bufferName) != m_buffers_map.end(), "No buffer with that name found");

        return buffers[m_buffers_map[bufferName]];
    }
};

#endif
