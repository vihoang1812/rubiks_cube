#ifndef KEYBOARD_H
#define KEYBOARD_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Keyboard {
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
}
#endif