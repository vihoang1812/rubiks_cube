#include <stdexcept>
#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "graphics.h"
#include "stb_image.h"
#include "constants.h"

uint Graphics::registerMesh(uint vertexDataSize, void* vertexData, uint indexDataSize, void* indexData) {
    if(vertexDataSize == 0) {
        throw new std::runtime_error("vertex size must be greater then zero");
    }

    if(indexDataSize == 0) {
        throw new std::runtime_error("index size must be greater than zero");
    }

    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo); //Get me a new buffer id (like a pointer) from GPU and store it in vbo variable
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size //HARD CODED - Should fix later
        GL_FLOAT,           // type //HARD CODED - Should fix later
        GL_FALSE,           // normalized?
        5 * sizeof(float),  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );


    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vao;
}

uint Graphics::registerTexture(str fileName) {
    if(!shaderProgramLoaded) {
        throw new std::runtime_error("shader program must be created and selected before loading textures");
    }

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << fileName << std::endl;
    }

    stbi_image_free(data);
    GLuint textureLocation = glGetUniformLocation(shaderProgramId, "ourTexture");
    glUniform1i(textureLocation, 0); 

    return textureID;
}

uint Graphics::registerShader(str fileName, uint shaderType) {
    GLuint shaderId = glCreateShader(shaderType);
    
    std::ifstream infile(fileName);
    std::string line, shaderCode;
    while (std::getline(infile, line)) {
        shaderCode += line + "\n";
    }
    
    const char* shaderSourceCstr = shaderCode.c_str();
    glShaderSource(shaderId, 1, &shaderSourceCstr, NULL);
    glCompileShader(shaderId);

    return shaderId;
}

uint Graphics::registerShaderProgram() {
    return glCreateProgram();
}

void Graphics::attachShader(uint shaderProgramId, uint shaderId) {
    glAttachShader(shaderProgramId, shaderId);
}

void Graphics::linkShader(uint shaderProgramId) {
    glLinkProgram(shaderProgramId);

    GLint success;
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if(!success) {
        GLchar infoLog[512];
        std::cout << "shader program failed to link" << std::endl;
        glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        throw new std::runtime_error("shader failed at link step");
    }
}

void Graphics::selectShaderProgram(uint shaderProgramId) {
    glUseProgram(shaderProgramId);
}

//Can simplify function call later
void Graphics::drawMesh(uint meshId, uint textureId, uint totalIndicies, float* model, float* view, float* projection) {
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shaderProgramId, "model");
    unsigned int viewLoc  = glGetUniformLocation(shaderProgramId, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgramId, "projection");

    if(model == nullptr) throw new std::runtime_error("model is null");
    if(view == nullptr) throw new std::runtime_error("view is null");
    if(projection == nullptr) throw new std::runtime_error("projection is null");

    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection);

    glActiveTexture(GL_TEXTURE0); // Activate texture unit
    glBindTexture(GL_TEXTURE_2D, textureId); // Bind the cube's texture

    if(meshId == 0) {
        throw new std::runtime_error("Drawable id is invalid");
    }

    glBindVertexArray(meshId);
    glDrawElements(GL_TRIANGLES, totalIndicies, GL_UNSIGNED_INT, 0);
}

Graphics& Graphics::getInstance() {
    static Graphics instance;
    return instance;
}