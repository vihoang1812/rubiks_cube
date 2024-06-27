#include "cube.h"

Cube::Cube(glm::vec3 curPos, StatePos curState, uint textureID) : curPos(curPos), curState(curState), textureID(textureID) {
    mesh = {
        {
        //top, yellow
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f,  0.75f, 0.5f},
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.25f},
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.25f},
        {-0.5f,  0.5f,  0.5f,  0.5f,  0.25f},
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.5f},

        // bottom black
        {-0.5f, -0.5f, -0.5f,  0.0f,   0.5f},
        { 0.5f, -0.5f, -0.5f,  0.25f,  0.5f},
        { 0.5f,  0.5f, -0.5f,  0.25f,  0.25f},
        { 0.5f,  0.5f, -0.5f,  0.25f,  0.25f},
        {-0.5f,  0.5f, -0.5f,  0.0f,   0.25f},
        {-0.5f, -0.5f, -0.5f,  0.0f,   0.5f},

        // front  red (square 5)
        {-0.5f, -0.5f, -0.5f,  0.5f,   0.75f},
        { 0.5f, -0.5f, -0.5f,  0.75f,  0.75f},
        { 0.5f, -0.5f,  0.5f,  0.75f,  0.5f},
        { 0.5f, -0.5f,  0.5f,  0.75f,  0.5f},
        {-0.5f, -0.5f,  0.5f,  0.5f,   0.5f},
        {-0.5f, -0.5f, -0.5f,  0.5f,   0.75f},  

        // Left blue (square 6)
        {-0.5f,  0.5f, -0.5f,  0.25f, 0.5f},
        {-0.5f,  0.5f,  0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.25f},
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.25f},
        {-0.5f, -0.5f, -0.5f,  0.25f, 0.25f},
        {-0.5f,  0.5f, -0.5f,  0.25f, 0.5f},

        // back face (square 7)
        {-0.5f,  0.5f,  0.5f, 0.5f,   0.25f}, 
        { 0.5f,  0.5f,  0.5f, 0.75f,  0.25f}, 
        { 0.5f,  0.5f, -0.5f, 0.75f,  0.0f}, 
        { 0.5f,  0.5f, -0.5f, 0.75f,  0.0f}, 
        {-0.5f,  0.5f, -0.5f, 0.5f,   0.0f}, 
        {-0.5f,  0.5f,  0.5f, 0.5f,   0.25f}, 

        // Right black (square 8)
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.5f},
        { 0.5f,  0.5f, -0.5f,  1.0f,  0.5f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.25f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.25f},
        { 0.5f, -0.5f,  0.5f,  0.75f, 0.25f},
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.5f}
        },
        
        //Index
        //Cube 1
        {0, 1, 2, 3, 4, 5,     // Top face
        6, 7, 8, 9, 10, 11,   // Bottom face
        12, 13, 14, 15, 16, 17, // Front face
        18, 19, 20, 21, 22, 23, // Left face
        24, 25, 26, 27, 28, 29, // Back face
        30, 31, 32, 33, 34, 35} // Right face
    };
}

void Cube::setState(StatePos nextState) {
    curState = nextState;
    curPos = PosFromState::getInstance()[nextState];
}

void Cube::rotateX(float angle) {
    glm::quat newRot = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    rot = newRot * rot;
}

void Cube::rotateY(float angle) {
    glm::quat newRot = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    rot = newRot * rot;
}

void Cube::rotateZ(float angle) {
    glm::quat newRot = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    rot = newRot * rot;
}

StatePos Cube::getState() {
    return curState;
}

const Mesh& Cube::getMesh() {
    return mesh;
}
