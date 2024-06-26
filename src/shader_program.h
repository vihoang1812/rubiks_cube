#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "common_alias.h"
#include "shader.h"

class ShaderProgram {
private:
    uint id;
public:
    ShaderProgram(vec<Shader> shaders);
    uint getId();
    void selectThisShader();
};

#endif