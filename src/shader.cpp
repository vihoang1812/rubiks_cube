#include "shader.h"
#include "graphics.h"

Shader::Shader(str fileName, uint shaderType) {
    id = Graphics::getInstance().registerShader(fileName, shaderType);
}

uint Shader::getId() {
    return id;
}