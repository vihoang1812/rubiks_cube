#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "observer.h"
#include "event.h"
#include "cube.h"
#include "camera.h"

class RubiksCube : public Observer{
private:
    std::vector<Cube> cubeData;
    Camera camera;

public:
    RubiksCube();
    void renderCube(Cube& cube);
    void draw();
    void onEvent(Event e) override;
};

#endif