#pragma once

#ifndef VBOMANAGER
#define VBOMANAGER

#include <iostream>
#include <variant>
#include <vector>

#include <GL/gl3w.h>

#include "vertex.h"

template <typename T> class VAOManager {

  private:
    std::size_t m_vertexSize;
    std::size_t m_vertexCount;

  protected:
  public:
    VAOManager() : m_vertexSize(0), m_vertexCount(0){};
    ~VAOManager(){};

    std::vector<T> vertexData;
    GLuint id;

    void addVertex(T _vertexData) {
        vertexData.push_back(_vertexData);
        m_vertexSize += sizeof(T);
        ++m_vertexCount;
    }

    std::size_t getVertexCount() { return m_vertexCount; }
    std::size_t getVertexSize() { return sizeof(T); }
    std::size_t getVertexDataSize() { return m_vertexSize; }
};

#endif