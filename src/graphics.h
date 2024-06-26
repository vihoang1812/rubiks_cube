#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common_alias.h"
#include "mesh.h"

class Graphics {
private:
    Graphics() = default;
    bool shaderProgramLoaded = false;
    uint shaderProgramId = 0;

public:
    uint registerMesh(uint vertexDataSize, void* vertexData, uint indexDataSize, void* indexData);
    uint registerTexture(str fileName);
    uint registerShader(str fileName, uint shaderType);
    uint registerShaderProgram();
    void attachShader(uint shaderProgramId, uint shaderId);
    void linkShader(uint shaderProgramId);
    void selectShaderProgram(uint shaderProgramId);
    void drawMesh(uint meshId, uint textureId, uint totalIndicies, float* model, float* view, float* projection);
    static Graphics& getInstance();

};

#endif