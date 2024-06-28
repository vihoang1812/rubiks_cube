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

    //Todo: Camera should be its own Class
    glm::vec3 cameraPos = glm::vec3(0.0f, -10.0f,  0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);    
    glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);

    float camRotX = 0;
    float camRotY = 0;
    float camRotZ = 0;

    float cameraSpeed = 0.5f;

public:
    RubiksCube();
    void renderCube(Cube& cube, glm::vec3 cameraPos);
    void draw();
    void onEvent(Event e) override;
};

#endif