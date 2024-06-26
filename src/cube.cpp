#include "cube.h"

Cube::Cube(glm::vec3 curPos, StatePos curState, uint textureID) : curPos(curPos), curState(curState), textureID(textureID) {

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
