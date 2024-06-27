#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common_alias.h"
#include "mesh.h"

class Graphics {
private:
    Graphics() = default;
    bool shaderProgramLoaded = false;
    uint shaderProgramId = 999;

public:
    Graphics(const Graphics&) = delete;

    uint registerMesh(uint vertexDataSize, void* vertexData, uint indexDataSize, void* indexData);
    uint registerTexture(str fileName);
    uint registerShader(str fileName, uint shaderType);
    uint registerShaderProgram();
    void attachShader(uint shaderProgramId, uint shaderId);
    void linkShader(uint shaderProgramId);
    void selectShaderProgram(uint shaderProgramId);
    uint getUniformId(str uniformName);
    void setUniformMat4fv(uint uniformLocation, float* data);
    void setUniformMat4fv(str uniformName, float* data);
    void setTexture(uint textureId);
    void drawMesh(uint meshId, uint totalIndicies);
    static Graphics& getInstance();

};

#endif