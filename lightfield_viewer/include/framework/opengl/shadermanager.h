#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <vector>

#include "qulkan/utils.h"

#include "framework/opengl/compiler.h"

class ShaderManager {

  private:
    std::map<std::string, GLuint> m_shaders_map;
    std::map<std::string, std::string> m_path_map;

  public:
    void addShader(std::string shaderName, std::string path, GLenum shaderType, Compiler &compiler) {

        m_shaders_map[shaderName] = compiler.create(shaderType, path, "--version 330 --profile core");

        m_path_map[shaderName] = path;
    }

    GLuint operator()(const std::string shaderName) { return shaderID(shaderName); }

    GLuint shaderID(const std::string shaderName) {
        ASSERT(m_shaders_map.find(shaderName) != m_shaders_map.end(), "No shader with that name found");

        return m_shaders_map[shaderName];
    }

    char const *shaderPath(const std::string shaderName) {
        ASSERT(m_path_map.find(shaderName) != m_path_map.end(), "No shader path with that name found");

        return m_path_map[shaderName].c_str();
    }
};

#endif
