#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "observer.h"
#include "rubiks_cube.h"
#include "shader_program.h"

class Window : public Observer {
private:
    GLFWwindow* window;
    RubiksCube* rubiksCube;
    ShaderProgram shaderProgram;
public:
    Window() = delete;
    Window(GLFWerrorfun errorCallBack, GLFWkeyfun keyCallBack);
    
    //TODO: This should be an event that is raised!
    bool isOpen();
    void focus();
    void update();
    void clear();
    void onEvent(Event e) override;
    void run();
};

#endif