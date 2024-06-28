#include "event_handler.h"
#include "camera.h"

Camera::Camera(){
    EventHandler::getInstance().subscribe(this);
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getFront() const {
    return front;
}

glm::vec3 Camera::getUp() const {
    return up;
}

float Camera::rotX() const{
    return camRotX;
}

float Camera::rotY() const{
    return camRotY;
}

float Camera::rotZ() const{
    return camRotZ;
}

void Camera::onEvent(Event e) {

    if (e == Event::CAMERA_ROTATE_DOWN) {
        camRotX += 0.1;
    }

    if (e == Event::CAMERA_ROTATE_UP) {
        //std::cout << "Up was Pressed :)" << std::endl; 
        camRotX -= 0.1;
    }

    if (e == Event::CAMERA_ROTATE_RIGHT) {
        //std::cout << "RIGHT_SHIFT was Pressed :)" << std::endl; 
        camRotZ += 0.1f;
    }

    if (e == Event::CAMERA_ROTATE_LEFT) {
        //std::cout << "LEFT_SHIFT was Pressed :)" << std::endl; 
        camRotZ -= 0.1f;
    }

    if (e == Event::CAMERA_ROTATE_ROLL_LEFT) {
        //std::cout << "Left was Pressed :)" << std::endl; 
        camRotY += 0.1f; 
    }

    if (e == Event::CAMERA_ROTATE_ROLL_RIGHT) {
        //std::cout << "Right was Pressed :)" << std::endl; 
        camRotY -= 0.1f; // Rotate around Y-axis (left and right)
    }

    if (e == Event::CAMERA_MOVE_FORWARDS) {
        position += speed * front;
    }

    if (e == Event::CAMERA_MOVE_BACKWARDS) {
        position -= speed * front;
    }

    if (e == Event::CAMERA_MOVE_LEFT) {
        position -= glm::normalize(glm::cross(front, up)) * speed;
    }

    if (e == Event::CAMERA_MOVE_RIGHT) {
        position += glm::normalize(glm::cross(front, up)) * speed;
    }
}