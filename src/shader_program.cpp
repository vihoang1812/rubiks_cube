#include "shader_program.h"
#include "graphics.h"

ShaderProgram::ShaderProgram(vec<Shader> shaders) {
    for(Shader shader : shaders) {
        Graphics::getInstance().attachShader(id, shader.getId());
    }
    Graphics::getInstance().linkShader(id);
}

uint ShaderProgram::getId() {
    return id;
}

void ShaderProgram::selectThisShader() {
    Graphics::getInstance().selectShaderProgram(id);
}