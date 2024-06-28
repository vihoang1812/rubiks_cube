#include <iostream>

#include "constants.h"
#include "window.h"
#include "event_handler.h"
#include "shader.h"
#include "graphics.h"

Window::Window(GLFWerrorfun errorCallBack, GLFWkeyfun keyCallBack) {
    std::cout << "Hello OpenGL" << std::endl;

    EventHandler::getInstance().subscribe(this);

    glfwSetErrorCallback(errorCallBack);

    //Init GLFW and ensure its not cooked
    if (!glfwInit()) throw std::runtime_error("GLFW Error");

    //OpenGL 4.1 is latest suppported for Mac
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Constants::OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Constants::OPENGL_MINOR_VERSION);

    std::cout << "Loaded OpenGL" << std::endl;

    window = glfwCreateWindow(Constants::SCR_WIDTH, Constants::SCR_HEIGHT, Constants::SRC_TITLE, NULL, NULL);

    //Make sure window is not cooked
    if(!window) throw std::runtime_error("Window Error");

    glfwSetKeyCallback(window, keyCallBack);
    
    focus();

    gladLoadGL();
    glfwSwapInterval(Constants::OPENGL_SWAP_INTERVAL);

    Shader vertexShader("../src/shader/vertex_shader.glsl", Shader::VERTEX_SHADER);
    Shader pixelShader("../src/shader/pixel_shader.glsl", Shader::PIXEL_SHADER);
    ShaderProgram newShaderProgram({vertexShader, pixelShader});
    shaderProgram = newShaderProgram;
    shaderProgram.selectThisShader();

    rubiksCube = new RubiksCube;
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(window);
}

void Window::focus() {
    glfwMakeContextCurrent(window);
}

void Window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    if(!isOpen()) EventHandler::getInstance().sendEvent(Event::WINDOW_CLOSE);
}

void Window::run() {
    while(isOpen()) {
        EventHandler::getInstance().checkEvents();
        
        clear();

        shaderProgram.selectThisShader();
        rubiksCube->draw();

        update();
    }
}

void Window::clear() {
    auto[R, G, B, T] = Constants::SCR_BACKGROUND_COLOUR;
    glClearColor(R, G, B, T);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::onEvent(Event e) {
    if(e == Event::WINDOW_CLOSE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}