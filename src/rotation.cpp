#include "rotation.h"

glm::mat4 Rotation::createModelMat(glm::vec3 position, glm::quat rotation) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * glm::mat4_cast(rotation);
    return model;
}

glm::mat4 Rotation::createViewMat(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float camRotX, float camRotY, float camRotZ) {
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view = glm::rotate(view, camRotX, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, camRotY, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, camRotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    return view;
}

glm::mat4 Rotation::createProjectionMat(float fov, float ratio, float nearDist, float farDist) {
    return glm::perspective(glm::radians(fov), ratio, nearDist, farDist);
}