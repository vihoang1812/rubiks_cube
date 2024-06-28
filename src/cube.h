#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common_alias.h"
#include "state.h"
#include "mesh.h"

class Cube {
public:
    float xRot = 0;
    float yRot = 0;
    float zRot = 0;
    glm::quat rot = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 curPos = {0.0f, 0.0f, 0.0f};
    StatePos initState;
    StatePos curState;
    uint textureID;
    Mesh mesh;
    
    Cube(glm::vec3 curPos, StatePos curState, uint textureID);
    void setState(StatePos nextState);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    StatePos getState();
    const Mesh& getMesh();
    void reset();
};

#endif