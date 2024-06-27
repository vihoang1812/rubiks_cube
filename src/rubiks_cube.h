#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "observer.h"
#include "event.h"
#include "cube.h"

class RubiksCube : public Observer{
private:
    std::vector<Cube> cubeData;
public:
    RubiksCube();
    void renderCube(Cube& cube, glm::vec3 cameraPos);
    void draw();
    void onEvent(Event e) override;
};

#endif