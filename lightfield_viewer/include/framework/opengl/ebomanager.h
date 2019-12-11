#pragma once

#ifndef EBOMANAGER
#define EBOMANAGER

#include <iostream>
#include <variant>
#include <vector>

#include <GL/gl3w.h>

#include "vertex.h"

//     GLsizei const elementCount(6);
//     GLsizeiptr const elementSize = elementCount * sizeof(GLushort);
//     GLushort const elementData[elementCount] = {
//         0, 1, 2, // triangle 1
//         2, 3, 0  // triangle 2
//     };

class EBOManager {

  private:
    std::size_t m_elementSize;
    std::size_t m_elementCount;

  protected:
  public:
    EBOManager() : m_elementSize(0), m_elementCount(0){};
    ~EBOManager(){};

    std::vector<GLuint> elementData;
    GLuint id;

    void addPoint(GLuint _elementData1) {
        elementData.push_back(_elementData1);
        m_elementSize += sizeof(GLuint);
        m_elementCount += 1;
    }

    void addLine(GLuint _elementData1, GLuint _elementData2) {
        elementData.push_back(_elementData1);
        elementData.push_back(_elementData2);
        m_elementSize += 2 * sizeof(GLuint);
        m_elementCount += 2;
    }

    void addTriangle(GLuint _elementData1, GLuint _elementData2, GLuint _elementData3) {
        elementData.push_back(_elementData1);
        elementData.push_back(_elementData2);
        elementData.push_back(_elementData3);
        m_elementSize += 3 * sizeof(GLuint);
        m_elementCount += 3;
    }

    void addElements(GLuint element) {
        elementData.push_back(element);
        m_elementSize += sizeof(GLuint);
        ++m_elementCount;
    }

    template <typename... Param>
    void addElements(GLuint element, Param... params) // recursive variadic function
    {
        elementData.push_back(element);
        m_elementSize += sizeof(GLuint);
        ++m_elementCount;
        addElements(params...);
    }

    std::size_t getElementCount() { return m_elementCount; }
    std::size_t getElementSize() { return m_elementSize; }
};

#endif