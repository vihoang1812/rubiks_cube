#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>
#include <stdexcept>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GL_PIXEL_SHADER GL_FRAGMENT_SHADER

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "cube.h"
#include "state.h"
#include "mesh.h"
#include "graphics.h"
#include "shader.h"
#include "shader_program.h"
#include "texture.h"
#include "rotation.h"
#include "constants.h"
#include "window.h"
#include "event_handler.h"
#include "event.h"

static Graphics& g = Graphics::getInstance();

glm::vec3 cameraPos   = glm::vec3(0.0f, -10.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);    
glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);

//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;
//float camRotX = 0;
//float camRotY = 0;
//float camRotZ = 0;

static void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float cameraSpeed = 0.5f; 

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        EventHandler::getInstance().sendEvent(Event::WINDOW_CLOSE);
    }

    // if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
    //     std::cout << "Enter was Pressed :)" << std::endl; 
    //     cameraPos += glm::vec3(0.1f, 0.0f, 0.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    //     //std::cout << "Down was Pressed :)" << std::endl; 
    //     camRotX += 0.1;
    // }

    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //     //std::cout << "Up was Pressed :)" << std::endl; 
    //     camRotX -= 0.1;
    // }

    // if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
    //     //std::cout << "RIGHT_SHIFT was Pressed :)" << std::endl; 
    //     camRotZ += 0.1f;
    // }

    // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    //     //std::cout << "LEFT_SHIFT was Pressed :)" << std::endl; 
    //     camRotZ -= 0.1f;
    // }

    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //     //std::cout << "Left was Pressed :)" << std::endl; 
    //     camRotY += 0.1f; 
    // }

    // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //     //std::cout << "Right was Pressed :)" << std::endl; 
    //     camRotY -= 0.1f; // Rotate around Y-axis (left and right)
    // }

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     cameraPos += cameraSpeed * cameraFront;

    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     cameraPos -= cameraSpeed * cameraFront;

    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_3 || 
    //             cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_7) {
    //             rotate_cube_right(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_3 || 
    //             cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_7) {
    //             rotate_cube_right_inv(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_1 || cube.getState() == StatePos::pos_2 || 
    //             cube.getState() == StatePos::pos_5 || cube.getState() == StatePos::pos_6) {
    //             rotate_cube_left(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_1 || cube.getState() == StatePos::pos_2 || 
    //             cube.getState() == StatePos::pos_5 || cube.getState() == StatePos::pos_6) {
    //             rotate_cube_left_inv(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
    //             cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3) {
    //             rotate_cube_up(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
    //             cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3) {
    //             rotate_cube_up_inv(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5 || 
    //             cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
    //             rotate_cube_bot(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5 || 
    //             cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
    //             rotate_cube_bot_inv(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3 || 
    //             cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
    //             rotate_cube_front(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3 || 
    //             cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
    //             rotate_cube_front_inv(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
    //             cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5) {
    //             rotate_cube_back(cube);
    //         }
    //     }
    //     printCubes();
    // }

    // if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
    //     for(auto& cube : cubeData) {
    //         if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
    //             cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5) {
    //             rotate_cube_back_inv(cube);
    //         }
    //     }
    //     printCubes();
    // }
}

int main() {

    Window window(error_callback, key_callback);

    EventHandler::getInstance().run();
    std::cout << "Main Thread Running" << std::endl;
    window.run();

    std::cout << "Window Closed" << std::endl;
}