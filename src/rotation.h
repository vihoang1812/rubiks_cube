#ifndef ROTATION_H
#define ROTATION_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Rotation {
    public:
        static glm::mat4 createModelMat(glm::vec3 position, glm::quat rotation);
        static glm::mat4 createViewMat(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float camRotX, float camRotY, float camRotZ);
        static glm::mat4 createProjectionMat(float fov, float ratio, float nearDist, float farDist);
};
#endif //ROTATION_H