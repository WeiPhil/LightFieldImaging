#pragma once

#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <vector>

#include "qulkan/utils.h"

#include "framework/opengl/compiler.h"

class ProgramManager {

  private:
    std::map<std::string, GLuint> m_programs_map;
    int m_max;

  public:
    ProgramManager() : m_max(0){};
    ~ProgramManager(){};

    int size() const { return m_max; }

    void addProgram(std::string programName) {
        m_programs_map[programName] = glCreateProgram();
        ++m_max;
    }

    GLuint operator()(const std::string programName) { return programID(programName); }

    GLuint programID(const std::string programName) {
        ASSERT(m_programs_map.find(programName) != m_programs_map.end(), "No program with that name found: " + programName);

        return m_programs_map[programName];
    }
};

#endif
