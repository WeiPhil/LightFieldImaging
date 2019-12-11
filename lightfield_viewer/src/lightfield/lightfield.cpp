#include "lightfield/lightfield.h"

#include <array>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "framework/opengl/compiler.h"
#include "framework/opengl/semantics.h"
#include "framework/opengl/vertex.h"
#include "qulkan/logger.h"
#include "qulkan/utils.h"
#include "utils/stb_image.h"

#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Lightfield::Lightfield(const char *viewName, int initialRenderWidth, int initialRenderHeight)
    : Qulkan::RenderView(viewName, initialRenderWidth, initialRenderHeight), x_pan(1.f), y_pan(1.f) {
    vaoManager.addVertex(glf::vertex_v3fv2f(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));   // top right
    vaoManager.addVertex(glf::vertex_v3fv2f(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));  // top left
    vaoManager.addVertex(glf::vertex_v3fv2f(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f))); // bottom left
    vaoManager.addVertex(glf::vertex_v3fv2f(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));  // bottom right

    eboManager.addTriangle(0, 1, 2);
    eboManager.addTriangle(2, 3, 0);
}

void Lightfield::initHandles() {

    Handle curr_image("Current Image", Type::INT, 0, 0, 5);
    Handle modeText("modeText", Type::TEXT, "(0 = Directional ,1 = Aperture, 2 = Refocus)");
    Handle mode("Mode", Type::INT, 0, 0, 2);
    Handle apertureText("Aperture", Type::TEXT, "\tAperture options:");
    Handle fstop("F-Stop", Type::INT, 9, 0, 9);
    Handle refocusText("Digital Refocus", Type::TEXT, "\tDigital Refocus options:");
    Handle alpha("Alpha", Type::FLOAT, 0.f, -3.f, 3.f);

    handleManager.addHandle(curr_image);
    handleManager.addHandle(modeText);

    handleManager.addHandle(mode);

    handleManager.addHandle(apertureText);
    handleManager.addHandle(fstop);

    handleManager.addHandle(refocusText);
    handleManager.addHandle(alpha);
    // handleManager.addHandle(interpolation);
}

void Lightfield::initProgram() {

    Compiler compiler;

    shaderManager.addShader("VERT_DEFAULT", "../data/shaders/lightfield.vert", GL_VERTEX_SHADER, compiler);
    shaderManager.addShader("FRAG_DEFAULT", "../data/shaders/lightfield.frag", GL_FRAGMENT_SHADER, compiler);

    programManager.addProgram("DEFAULT");

    glAttachShader(programManager("DEFAULT"), shaderManager("VERT_DEFAULT"));
    glAttachShader(programManager("DEFAULT"), shaderManager("FRAG_DEFAULT"));
    glLinkProgram(programManager("DEFAULT"));

    error = compiler.check() && error;
    error = compiler.check_program(programManager("DEFAULT")) && error;

    return;
}

void Lightfield::initBuffer() {

    bufferManager.addBuffer("ELEMENT");
    bufferManager.addBuffer("VERTEX");

    glGenBuffers(bufferManager.size(), &bufferManager.buffers[0]);

    // Create vertex array object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferManager("ELEMENT"));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboManager.getElementSize(), &eboManager.elementData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, bufferManager("VERTEX"));
    glBufferData(GL_ARRAY_BUFFER, vaoManager.getVertexDataSize(), &vaoManager.vertexData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return;
}

void Lightfield::initTexture() {

    textureManager.addTexture("IMAGE_LIGHTFIELD1", "../data/images/lightfield/fluor.jpg");
    textureManager.addTexture("IMAGE_LIGHTFIELD2", "../data/images/lightfield/lego.jpg");
    textureManager.addTexture("IMAGE_LIGHTFIELD3", "../data/images/lightfield/bunny.jpg");
    textureManager.addTexture("IMAGE_LIGHTFIELD4", "../data/images/lightfield/crystal.jpg");

    glGenTextures(textureManager.size(), &textureManager.textures[0]);

    unsigned char *texData;
    int width, height, nrChannels;
    std::cout << "[Lightfield Imaging]: Loading images, this can take time.." << std::endl;
    for (size_t i = 0; i < 4; i++) {
        std::string texture_path = "IMAGE_LIGHTFIELD" + std::to_string(i + 1);

        glBindTexture(GL_TEXTURE_RECTANGLE, textureManager(texture_path));
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        texData = stbi_load(textureManager.texturePath(texture_path), &width, &height, &nrChannels, 0);
        image_dims_side[i] = glm::vec3(width, height, 17);

        if (texData) {
            glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        } else {
            Qulkan::Logger::Error("Failed to load texture %d", i);
        }
        stbi_image_free(texData);
    }
    std::cout << "[Lightfield Imaging]: Done!" << std::endl;
    texture_id_program_id[0] = {GL_TEXTURE0, textureManager("IMAGE_LIGHTFIELD1")};
    image_dims_side[0].z = 20;
    texture_id_program_id[1] = {GL_TEXTURE1, textureManager("IMAGE_LIGHTFIELD2")};
    texture_id_program_id[2] = {GL_TEXTURE2, textureManager("IMAGE_LIGHTFIELD3")};
    texture_id_program_id[3] = {GL_TEXTURE3, textureManager("IMAGE_LIGHTFIELD4")};

    glBindTexture(GL_TEXTURE_RECTANGLE, 0);

    return;
}

void Lightfield::initVertexArray() {
    glGenVertexArrays(1, &vaoManager.id);
    glBindVertexArray(vaoManager.id);
    glBindBuffer(GL_ARRAY_BUFFER, bufferManager("VERTEX"));

    glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, vaoManager.getVertexSize(), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(semantic::attr::POSITION);

    glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, vaoManager.getVertexSize(), BUFFER_OFFSET(sizeof(glm::vec3)));
    glEnableVertexAttribArray(semantic::attr::TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind element buffer array to array ob
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferManager("ELEMENT"));
    glBindVertexArray(0);

    return;
}

void Lightfield::initFramebuffer() { return; }

void Lightfield::init() {
    Qulkan::Logger::Info("%s: Initialisation\n", name());

    if (handleManager.getHandles().empty())
        initHandles();
    initProgram();
    initBuffer();
    initTexture();
    initVertexArray();
    initFramebuffer();
    initOpenGLOptions();
    if (!error) {
        Qulkan::Logger::Info("%s: Initialisation Done\n", name());
        initialized = true;
    } else
        Qulkan::Logger::Error("%s: An error Occured during initialisation\n", name());
}

void Lightfield::initOpenGLOptions() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Lightfield::clean() {
    glDeleteFramebuffers(framebufferManager.size(), &framebufferManager.framebuffers[0]);
    glDeleteProgram(programManager("DEFAULT"));

    glDeleteBuffers(bufferManager.size(), &bufferManager.buffers[0]);
    glDeleteTextures(textureManager.size(), &textureManager.textures[0]);
    glDeleteVertexArrays(1, &vaoManager.id);
}

/* Renders a simple OpenGL triangle in the rendering view */
void Lightfield::render(int actualRenderWidth, int actualRenderHeight) {
    ASSERT(initialized, std::string(name()) + ": You need to init the view first");

    glViewport(0, 0, actualRenderWidth, actualRenderHeight);

    glClearColor(0.233f, 0.233f, 0.233f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(programManager("DEFAULT"));

    auto current_image = handleManager("Current Image")->getValue<int>();

    glActiveTexture(std::get<0>(texture_id_program_id[current_image]));
    glBindTexture(GL_TEXTURE_RECTANGLE, std::get<1>(texture_id_program_id[current_image]));

    glUniform3f(glGetUniformLocation(programManager("DEFAULT"), "image_properties"), image_dims_side[current_image].x, image_dims_side[current_image].y,
                image_dims_side[current_image].z);
    glUniform1i(glGetUniformLocation(programManager("DEFAULT"), "texture1"), current_image);

    float sensibility = 0.5;
    if ((isKeyDown(GLFW_KEY_W) || isKeyDown(GLFW_KEY_UP)) && y_pan < image_dims_side[current_image].z - 1)
        y_pan += sensibility;
    if ((isKeyDown(GLFW_KEY_S) || isKeyDown(GLFW_KEY_DOWN)) && y_pan > 1)
        y_pan -= sensibility;
    if ((isKeyDown(GLFW_KEY_D) || isKeyDown(GLFW_KEY_RIGHT)) && x_pan < image_dims_side[current_image].z - 1)
        x_pan += sensibility;
    if ((isKeyDown(GLFW_KEY_A) || isKeyDown(GLFW_KEY_LEFT)) && x_pan > 1)
        x_pan -= sensibility;

    if (isMouseDragging(0)) {
        x_pan += mouseDelta.x * sensibility * 0.3f * Qulkan::getDeltaTime();
        y_pan += mouseDelta.y * sensibility * 0.3f * Qulkan::getDeltaTime();

        x_pan = std::clamp(x_pan, 1.0f, image_dims_side[current_image].z - 1.f);
        y_pan = std::clamp(y_pan, 1.0f, image_dims_side[current_image].z - 1.f);
    }

    int *fstop = std::any_cast<int>(&handleManager("F-Stop")->value);
    fstop_float = (float)handleManager("F-Stop")->getValue<int>();
    if (mouseWheel != 0.0f) {
        fstop_float -= mouseWheel;
    }
    if (fstop_float > 0.5f * image_dims_side[current_image].z - 1) {
        fstop_float = std::floor(0.5f * (image_dims_side[current_image].z - 1));
    }
    if (fstop_float < 0.f) {
        fstop_float = 0.f;
    }
    *fstop = (int)fstop_float;

    glUniform1i(glGetUniformLocation(programManager("DEFAULT"), "mode"), handleManager("Mode")->getValue<int>());
    glUniform1f(glGetUniformLocation(programManager("DEFAULT"), "x_pan"), x_pan);
    glUniform1f(glGetUniformLocation(programManager("DEFAULT"), "y_pan"), y_pan);
    glUniform1f(glGetUniformLocation(programManager("DEFAULT"), "alpha"), handleManager("Alpha")->getValue<float>());
    glUniform1i(glGetUniformLocation(programManager("DEFAULT"), "fstop"), (int)*fstop);

    glBindVertexArray(vaoManager.id);
    glDrawElementsInstancedBaseVertex(GL_TRIANGLES, eboManager.getElementCount(), GL_UNSIGNED_INT, 0, 1, 0);

    return;
}
