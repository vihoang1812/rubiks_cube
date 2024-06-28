#include "keyboard.h"
#include "event_handler.h"

namespace Keyboard {
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::WINDOW_CLOSE);
        }

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_RESET);
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_ROTATE_DOWN);
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_ROTATE_UP);
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_ROTATE_RIGHT);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_ROTATE_LEFT);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_ROTATE_ROLL_LEFT);
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_ROTATE_ROLL_RIGHT);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_MOVE_FORWARDS);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_MOVE_BACKWARDS);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_MOVE_LEFT);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::CAMERA_MOVE_RIGHT);
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_RIGHT);
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_RIGHT_INV);
        }

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_LEFT);
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_LEFT_INV);
        }

        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_UP);
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_UP_INV);
        }

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_BOTTOM);
        }

        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_BOTTOM_INV);
        }

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_FRONT);
        }

        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_FRONT_INV);
        }

        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_BACK);
        }

        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
            EventHandler::getInstance().sendEvent(Event::RUBIK_ROTATE_BACK_INV);
        }
    }
}