#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "observer.h"

class Camera : public Observer {
    private:
        glm::vec3 position = glm::vec3(0.0f, -10.0f, 0.0f);
        glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f);    
        glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

        float camRotX = 0;
        float camRotY = 0;
        float camRotZ = 0;

        float speed = 0.5f;

    public:
        Camera();
        glm::vec3 getPosition() const;
        glm::vec3 getFront() const;
        glm::vec3 getUp() const;
        float rotX() const;
        float rotY() const;
        float rotZ() const;
        void onEvent(Event e) override;

};
#endif // CAMERA_H