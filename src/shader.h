#ifndef SHADER_H
#define SHADER_H

#include "common_alias.h"

class Shader {
private:
    uint id;
public:
    static constexpr uint VERTEX_SHADER = 0x8b31;
    static constexpr uint PIXEL_SHADER = 0x8B30;

    Shader(str fileName, uint shaderType);
    uint getId();
};

#endif