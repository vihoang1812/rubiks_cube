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

//WINDOW SIZE
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//glm::vec3 camera = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);    
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//float rotateX = 0.0f;
//float rotateY = 0.0f;
//float rotateZ = 0.0f;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float camRotX = 0;
float camRotY = 0;
float camRotZ = 0;

//float cubeRotAng = 0.0f;

//double rad(double deg) {
//    return deg * (M_PI / 180.0);
//}

//Hold our vertex information
// * the location for each of our verticies
// * the ordering for how we should draw (indexData)

class Mesh {
private:
    std::vector<float> vertexData;
    std::vector<uint32_t> indexData;
public:
    Mesh() = default;
    Mesh(const std::vector<std::vector<float>>& vertexData, const std::vector<uint32_t>& indexData) {
        for(auto row : vertexData) {
            for(auto e : row) {
                this->vertexData.push_back(e);        
            }
        }

        for(auto e : indexData) this->indexData.push_back(e);
    }

    size_t getVertexDataSize() const {
        return vertexData.size() * sizeof(float);
    }

    size_t getIndexDataSize() const {
        return indexData.size() * sizeof(uint32_t);
    }

    std::vector<float> getVertexData() const {
        return vertexData;
    }
    std::vector<uint32_t> getIndexData() const {
        return indexData;
    }

    size_t getNumIndicies() {
        return indexData.size();
    }
};

enum class StatePos {
    pos_0,
    pos_1,
    pos_2,
    pos_3,
    pos_4,
    pos_5,
    pos_6,
    pos_7
};

struct PosFromState {
    glm::vec3 operator[](StatePos pos) {
        switch(pos) {
            case StatePos::pos_0: return glm::vec3( 0.5f,  0.5f,  0.5f);
            case StatePos::pos_1: return glm::vec3(-0.5f,  0.5f,  0.5f);
            case StatePos::pos_2: return glm::vec3(-0.5f, -0.5f,  0.5f);
            case StatePos::pos_3: return glm::vec3( 0.5f, -0.5f,  0.5f);
            case StatePos::pos_4: return glm::vec3( 0.5f,  0.5f, -0.5f);
            case StatePos::pos_5: return glm::vec3(-0.5f,  0.5f, -0.5f);
            case StatePos::pos_6: return glm::vec3(-0.5f, -0.5f, -0.5f);
            case StatePos::pos_7: return glm::vec3( 0.5f, -0.5f, -0.5f);
        }
    }
};
PosFromState posFromState;

struct Cube {
    float xRot = 0;
    float yRot = 0;
    float zRot = 0;
    glm::vec3 curPos = {0.0f, 0.0f, 0.0f};
    StatePos curState;
    GLuint textureID;
    Cube(glm::vec3 curPos, StatePos curState, GLuint textureID) : curPos(curPos), curState(curState), textureID(textureID) {}
    void setState(StatePos nextState) {
        curState = nextState;
        curPos = posFromState[nextState];
    }

    StatePos getState() {
        return curState;
    }
};

std::vector<Cube> cubeData;

// std::map<StatePos, std::any> posFromState;

// posFromState[StatePos::pos_0] = glm::vec3( 0.5f,  0.5f,  0.5f);
// posFromState[StatePos::pos_1] = glm::vec3(-0.5f,  0.5f,  0.5f); 
// posFromState[StatePos::pos_2] = glm::vec3(-0.5f, -0.5f,  0.5f);
// posFromState[StatePos::pos_3] = glm::vec3( 0.5f, -0.5f,  0.5f);
// posFromState[StatePos::pos_4] = glm::vec3( 0.5f,  0.5f, -0.5f);
// posFromState[StatePos::pos_5] = glm::vec3(-0.5f,  0.5f, -0.5f);
// posFromState[StatePos::pos_6] = glm::vec3(-0.5f, -0.5f, -0.5f);
// posFromState[StatePos::pos_7] = glm::vec3( 0.5f, -0.5f, -0.5f);

static void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

static void rotate_cube_x(Cube& cube) {
    cube.xRot -= 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_3);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_x_inv(Cube& cube) {
    cube.xRot += 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_4);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_y(Cube& cube) {
    cube.yRot -= 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_3);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_y_inv(Cube& cube) {
    cube.yRot += 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_6);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_z(Cube& cube) {
    cube.zRot += 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_4);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_z_inv(Cube& cube) {
    cube.zRot -= 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_6);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_right(Cube& cube) {
    cube.xRot -= 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_4);
            break;
        // case StatePos::pos_1:
        //     cube.setState(StatePos::pos_1);
        //     break;
        // case StatePos::pos_2:
        //     cube.setState(StatePos::pos_2);
        //     break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_7);
            break;
        // case StatePos::pos_5:
        //     cube.setState(StatePos::pos_5);
        //     break;
        // case StatePos::pos_6:
        //     cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_3);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_right_inv(Cube& cube) {
    cube.xRot += 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_3);
            break;
        // case StatePos::pos_1:
        //     cube.setState(StatePos::pos_1);
        //     break;
        // case StatePos::pos_2:
        //     cube.setState(StatePos::pos_2);
        //     break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_0);
            break;
        // case StatePos::pos_5:
        //     cube.setState(StatePos::pos_5);
        //     break;
        // case StatePos::pos_6:
        //     cube.setState(StatePos::pos_6);
        //     break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_4);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_left(Cube& cube) {
    cube.xRot -= 90;
    switch(cube.curState) {
        // case StatePos::pos_0:
        //     cube.setState(StatePos::pos_0);
        //     break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_1);
            break;
        // case StatePos::pos_3:
        //     cube.setState(StatePos::pos_3);
        //     break;
        // case StatePos::pos_4:
        //     cube.setState(StatePos::pos_4);
        //     break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_2);
            break;
        // case StatePos::pos_7:
        //     cube.setState(StatePos::pos_7);
        //     break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_left_inv(Cube& cube) {
    cube.xRot += 90;
    switch(cube.curState) {
        // case StatePos::pos_0:
        //     cube.setState(StatePos::pos_0);
        //     break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_6);
            break;
        // case StatePos::pos_3:
        //     cube.setState(StatePos::pos_3);
        //     break;
        // case StatePos::pos_4:
        //     cube.setState(StatePos::pos_4);
        //     break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_5);
            break;
        // case StatePos::pos_7:
        //     cube.setState(StatePos::pos_7);
        //     break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_up(Cube& cube) {
    cube.zRot -= 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_2);
            break;
        // case StatePos::pos_4:
        //     cube.setState(StatePos::pos_4);
        //     break;
        // case StatePos::pos_5:
        //     cube.setState(StatePos::pos_5);
        //     break;
        // case StatePos::pos_6:
        //     cube.setState(StatePos::pos_6);
        //     break;
        // case StatePos::pos_7:
        //     cube.setState(StatePos::pos_7);
        //     break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_up_inv(Cube& cube) {
    cube.zRot += 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_0);
            break;
        // case StatePos::pos_4:
        //     cube.setState(StatePos::pos_4);
        //     break;
        // case StatePos::pos_5:
        //     cube.setState(StatePos::pos_5);
        //     break;
        // case StatePos::pos_6:
        //     cube.setState(StatePos::pos_6);
        //     break;
        // case StatePos::pos_7:
        //     cube.setState(StatePos::pos_7);
        //     break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_bot(Cube& cube) {
    cube.zRot += 90;
    switch(cube.curState) {
        // case StatePos::pos_0:
        //     cube.setState(StatePos::pos_0);
        //     break;
        // case StatePos::pos_1:
        //     cube.setState(StatePos::pos_1);
        //     break;
        // case StatePos::pos_2:
        //     cube.setState(StatePos::pos_2);
        //     break;
        // case StatePos::pos_3:
        //     cube.setState(StatePos::pos_3);
        //     break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_4);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_bot_inv(Cube& cube) {
    cube.zRot -= 90;
    switch(cube.curState) {
        // case StatePos::pos_0:
        //     cube.setState(StatePos::pos_0);
        //     break;
        // case StatePos::pos_1:
        //     cube.setState(StatePos::pos_1);
        //     break;
        // case StatePos::pos_2:
        //     cube.setState(StatePos::pos_2);
        //     break;
        // case StatePos::pos_3:
        //     cube.setState(StatePos::pos_3);
        //     break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_6);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_front(Cube& cube) {
    cube.yRot -= 90;
    switch(cube.curState) {
        // case StatePos::pos_0:
        //     cube.setState(StatePos::pos_0);
        //     break;
        // case StatePos::pos_1:
        //     cube.setState(StatePos::pos_1);
        //     break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_3);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_front_inv(Cube& cube) {
    cube.yRot += 90;
    switch(cube.curState) {
        // case StatePos::pos_0:
        //     cube.setState(StatePos::pos_0);
        //     break;
        // case StatePos::pos_1:
        //     cube.setState(StatePos::pos_1);
        //     break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_7);
            break;
        // case StatePos::pos_4:
        //     cube.setState(StatePos::pos_4);
        //     break;
        // case StatePos::pos_5:
        //     cube.setState(StatePos::pos_5);
        //     break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_6);
            break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_back(Cube& cube) {
    cube.yRot -= 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_5);
            break;
        // case StatePos::pos_2:
        //     cube.setState(StatePos::pos_2);
        //     break;
        // case StatePos::pos_3:
        //     cube.setState(StatePos::pos_3);
        //     break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_4);
            break;
        // case StatePos::pos_6:
        //     cube.setState(StatePos::pos_6);
        //     break;
        // case StatePos::pos_7:
        //     cube.setState(StatePos::pos_7);
        //     break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void rotate_cube_back_inv(Cube& cube) {
    cube.yRot += 90;
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_0);
            break;
        // case StatePos::pos_2:
        //     cube.setState(StatePos::pos_2);
        //     break;
        // case StatePos::pos_3:
        //     cube.setState(StatePos::pos_3);
        //     break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_1);
            break;
        // case StatePos::pos_6:
        //     cube.setState(StatePos::pos_6);
        //     break;
        // case StatePos::pos_7:
        //     cube.setState(StatePos::pos_7);
        //     break;
        default:
            throw new std::runtime_error("undefined state");
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float cameraSpeed = 0.5f; 

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        std::cout << "Enter was Pressed :)" << std::endl; 
        cameraPos += glm::vec3(0.1f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::cout << "Down was Pressed :)" << std::endl; 
        camRotX += 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::cout << "Up was Pressed :)" << std::endl; 
        camRotX -= 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        std::cout << "RIGHT_SHIFT was Pressed :)" << std::endl; 
        camRotZ += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        std::cout << "LEFT_SHIFT was Pressed :)" << std::endl; 
        camRotZ -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        std::cout << "Left was Pressed :)" << std::endl; 
        camRotY += 0.1f; 
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        std::cout << "Right was Pressed :)" << std::endl; 
        camRotY -= 0.1f; // Rotate around Y-axis (left and right)
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        //rotateX += 90.0f;
        for(auto& cube : cubeData) rotate_cube_x(cube);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        //rotateX -= 90.0f; 
        for(auto& cube : cubeData) rotate_cube_z_inv(cube);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        //rotateY -= 90.0f; 
        for(auto& cube : cubeData) rotate_cube_y(cube);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        //rotateY += 90.0f;
        for(auto& cube : cubeData) rotate_cube_y_inv(cube);
    }
        
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        //rotateZ += 90.0f;
        for(auto& cube : cubeData) rotate_cube_z(cube);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        //rotateZ -= 90.0f;
        for(auto& cube : cubeData) rotate_cube_z_inv(cube); 
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_7) {
                rotate_cube_right(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_7) {
                rotate_cube_right_inv(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_1 || cube.getState() == StatePos::pos_2 || 
                cube.getState() == StatePos::pos_5 || cube.getState() == StatePos::pos_6) {
                rotate_cube_left(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_1 || cube.getState() == StatePos::pos_2 || 
                cube.getState() == StatePos::pos_5 || cube.getState() == StatePos::pos_6) {
                rotate_cube_left_inv(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3) {
                rotate_cube_up(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3) {
                rotate_cube_up_inv(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_bot(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_bot_inv(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_front(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_front_inv(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5) {
                rotate_cube_back(cube);
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5) {
                rotate_cube_back_inv(cube);
            }
        }
    }
}

GLuint registerShader(GLuint shaderType, std::string fileName) {
    GLuint shaderId = glCreateShader(shaderType);
    
    std::ifstream infile(fileName);
    std::string line, shaderCode;
    while (std::getline(infile, line)) {
        shaderCode += line + "\n";
    }
    
    const char* shaderSourceCstr = shaderCode.c_str();
    glShaderSource(shaderId, 1, &shaderSourceCstr, NULL);
    glCompileShader(shaderId);

    return shaderId;
}

GLuint registerShaderProgram() {
    return glCreateProgram();
}

void attachShader(GLuint programId, GLuint shaderId) {
    glAttachShader(programId, shaderId);
}

void linkShader(GLuint programId) {
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if(!success) {
        GLchar infoLog[512];
        std::cout << "shader program failed to link" << std::endl;
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }
}

void selectShaderProgram(GLuint programId) {
    glUseProgram(programId);
}

GLuint registerMesh(const Mesh& mesh) {
    if(mesh.getVertexDataSize() == 0) {
        throw new std::runtime_error("vertex size must be greater then zero");
    }

    if(mesh.getIndexDataSize() == 0) {
        throw new std::runtime_error("index size must be greater than zero");
    }

    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo); //Get me a new buffer id (like a pointer) from GPU and store it in vbo variable
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertexDataSize(), &(mesh.getVertexData())[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size //HARD CODED - Should fix later
        GL_FLOAT,           // type //HARD CODED - Should fix later
        GL_FALSE,           // normalized?
        5 * sizeof(float),  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );


    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexDataSize(), &(mesh.getIndexData())[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vao;
}

void renderCube(Cube cube, glm::vec3 cameraPos, GLuint shaderProgram) {
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    //view  = glm::translate(view, cameraPos);
    view = glm::rotate(view, camRotX, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, camRotY, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, camRotZ, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 objPos = cube.curPos;

    model = glm::translate(model, objPos);

    // // X-axis rotation
    
    // float x1 = objPos.x;
    // float y1 = objPos.y * cos(glm::radians(rotateX)) - objPos.z * sin(glm::radians(rotateX));
    // float z1 = objPos.y * sin(glm::radians(rotateX)) + objPos.z * cos(glm::radians(rotateX));

    // // Y-axis rotation
    // float x2 = x1 * cos(glm::radians(rotateY)) + objPos.z * sin(glm::radians(rotateY));
    // float y2 = y1;
    // float z2 = -x1 * sin(glm::radians(rotateY)) + objPos.z * cos(glm::radians(rotateY));

    // // // Z-axis rotation
    
    // float x_new = x2 * cos(glm::radians(rotateZ)) - y2 * sin(glm::radians(rotateZ));
    // float y_new = x2 * sin(glm::radians(rotateZ)) + y2 * cos(glm::radians(rotateZ));
    // float z_new = z2;

    // // // Update object position
    // objPos.x = x_new;
    // objPos.y = y_new;
    // objPos.z = z_new;

    model = glm::rotate(model, glm::radians(cube.xRot), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
    model = glm::rotate(model, glm::radians(cube.yRot), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
    model = glm::rotate(model, glm::radians(cube.zRot), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis
    
    //Z ROTATION
    // float r = sqrt(2.0f)/2;
    // float x = r * cos(cubeRotAng + initAngle);
    // float y = r * sin(cubeRotAng + initAngle);

    // float rotAngle = 45.0f;

    // float x_new = x * cos(glm::radians(rotAngle)) - y * sin(glm::radians(rotAngle));
    // float y_new = x * sin(glm::radians(rotAngle)) + y * cos(glm::radians(rotAngle));

    // model = glm::translate(model, glm::vec3(x_new, y_new, 0.0f));
    // model = glm::rotate(model, cubeRotAng + initAngle, glm::vec3(0.0f, 0.0f, 1.0f));  

    //Y ROTATION - FRONT
    // float r = sqrt(2.0f)/2;
    // float z = r * cos(cubeRotAng + initAngle);
    // float x = r * sin(cubeRotAng + initAngle);

    // float rotAngle = 45.0f;

    // float x_new = x * cos(glm::radians(rotAngle)) + z * sin(glm::radians(rotAngle));
    // float z_new = -x * sin(glm::radians(rotAngle)) + z * cos(glm::radians(rotAngle));

    // model = glm::translate(model, glm::vec3(x_new, 0.0f, z_new));
    // model = glm::rotate(model, cubeRotAng + initAngle , glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis
   
    // // X ROTATION - SIDE
    // float r = sqrt(2.0f)/2;
    // float y = r * cos(cubeRotAng + initAngle);
    // float z = r * sin(cubeRotAng + initAngle);

    // float rotAngle = 45.0f; 

    // float y_new = y * cos(glm::radians(rotAngle)) - z * sin(glm::radians(rotAngle));
    // float z_new = y * sin(glm::radians(rotAngle)) + z * cos(glm::radians(rotAngle));

    // model = glm::translate(model, glm::vec3(0.0f, y_new, z_new));
    // model = glm::rotate(model, cubeRotAng + initAngle, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around x-axis

    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0); // Activate texture unit
    glBindTexture(GL_TEXTURE_2D, cube.textureID); // Bind the cube's texture
}

void drawMesh(GLuint drawableId, GLuint totalIndicies) {
    if(drawableId == 0) {
        throw new std::runtime_error("Drawable id is invalid");
    }

    glBindVertexArray(drawableId);
    glDrawElements(GL_TRIANGLES, totalIndicies, GL_UNSIGNED_INT, 0);
}

GLuint loadTexture(const char* path, GLuint shaderProgram) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << path << std::endl;
    }

    stbi_image_free(data);
    GLuint textureLocation = glGetUniformLocation(shaderProgram, "ourTexture");
    glUniform1i(textureLocation, 0); 

    return textureID;
}

int main() {
    std::cout << "Hello OpenGL" << std::endl;

    glfwSetErrorCallback(error_callback);

    //Init GLFW and ensure its not cooked
    if (!glfwInit()) throw new std::runtime_error("GLFW Error");

    //OpenGL 4.1 is latest suppported for Mac
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    std::cout << "Loaded OpenGL" << std::endl;

    //Create window
    // const unsigned float SCR_WIDTH = 800;
    // const unsigned float SCR_HEIGHT = 600;
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rubics Cube", NULL, NULL);

    //Make sure window is not cooked
    if(!window) throw new std::runtime_error("Window Error");

    glfwSetKeyCallback(window, key_callback);
    
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    // Do Shader Stuff
    GLuint vertexShader = registerShader(GL_VERTEX_SHADER, "../src/shader/vertex_shader.glsl");
    GLuint pixelShader = registerShader(GL_PIXEL_SHADER, "../src/shader/pixel_shader.glsl");
    GLuint shaderProgram = registerShaderProgram();
    attachShader(shaderProgram, vertexShader);
    attachShader(shaderProgram, pixelShader);
    linkShader(shaderProgram);

    Mesh raw_mesh = {
        {
        //Cube 1
        
        //top, yellow
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f,  0.75f, 0.5f},
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.25f},
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.25f},
        {-0.5f,  0.5f,  0.5f,  0.5f,  0.25f},
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.5f},

        // bottom black
        {-0.5f, -0.5f, -0.5f,  0.0f,   0.5f},
        { 0.5f, -0.5f, -0.5f,  0.25f,  0.5f},
        { 0.5f,  0.5f, -0.5f,  0.25f,  0.25f},
        { 0.5f,  0.5f, -0.5f,  0.25f,  0.25f},
        {-0.5f,  0.5f, -0.5f,  0.0f,   0.25f},
        {-0.5f, -0.5f, -0.5f,  0.0f,   0.5f},

        // front  red (square 5)
        {-0.5f, -0.5f, -0.5f,  0.5f,   0.75f},
        { 0.5f, -0.5f, -0.5f,  0.75f,  0.75f},
        { 0.5f, -0.5f,  0.5f,  0.75f,  0.5f},
        { 0.5f, -0.5f,  0.5f,  0.75f,  0.5f},
        {-0.5f, -0.5f,  0.5f,  0.5f,   0.5f},
        {-0.5f, -0.5f, -0.5f,  0.5f,   0.75f},  

        // Left blue (square 6)
        {-0.5f,  0.5f, -0.5f,  0.25f, 0.5f},
        {-0.5f,  0.5f,  0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.25f},
        {-0.5f, -0.5f,  0.5f,  0.5f,  0.25f},
        {-0.5f, -0.5f, -0.5f,  0.25f, 0.25f},
        {-0.5f,  0.5f, -0.5f,  0.25f, 0.5f},

        // back face (square 7)
        {-0.5f,  0.5f,  0.5f, 0.5f,   0.25f}, 
        { 0.5f,  0.5f,  0.5f, 0.75f,  0.25f}, 
        { 0.5f,  0.5f, -0.5f, 0.75f,  0.0f}, 
        { 0.5f,  0.5f, -0.5f, 0.75f,  0.0f}, 
        {-0.5f,  0.5f, -0.5f, 0.5f,   0.0f}, 
        {-0.5f,  0.5f,  0.5f, 0.5f,   0.25f}, 

        // Right black (square 8)
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.5f},
        { 0.5f,  0.5f, -0.5f,  1.0f,  0.5f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.25f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.25f},
        { 0.5f, -0.5f,  0.5f,  0.75f, 0.25f},
        { 0.5f,  0.5f,  0.5f,  0.75f, 0.5f}

        //Cube 2
        // {0.5f, -0.5f, -0.5f,  0.0f, 0.0f},  // 36
        // {1.5f, -0.5f, -0.5f,  1.0f, 0.0f},  // 37
        // {1.5f,  0.5f, -0.5f,  1.0f, 1.0f},  // 38
        // {1.5f,  0.5f, -0.5f,  1.0f, 1.0f},  // 39
        // {0.5f,  0.5f, -0.5f,  0.0f, 1.0f},  // 40
        // {0.5f, -0.5f, -0.5f,  0.0f, 0.0f},  // 41

        // {0.5f, -0.5f,  0.5f,  0.0f, 0.0f},  // 42
        // {1.5f, -0.5f,  0.5f,  1.0f, 0.0f},  // 43
        // {1.5f,  0.5f,  0.5f,  1.0f, 1.0f},  // 44
        // {1.5f,  0.5f,  0.5f,  1.0f, 1.0f},  // 45
        // {0.5f,  0.5f,  0.5f,  0.0f, 1.0f},  // 46
        // {0.5f, -0.5f,  0.5f,  0.0f, 0.0f},  // 47

        // {1.5f, -0.5f, -0.5f,  0.0f, 0.0f},  // 48
        // {1.5f,  0.5f, -0.5f,  1.0f, 0.0f},  // 49
        // {1.5f,  0.5f,  0.5f,  1.0f, 1.0f},  // 50
        // {1.5f,  0.5f,  0.5f,  1.0f, 1.0f},  // 51
        // {1.5f, -0.5f,  0.5f,  0.0f, 1.0f},  // 52
        // {1.5f, -0.5f, -0.5f,  0.0f, 0.0f},  // 53

        // {1.5f, -0.5f, -0.5f,  0.0f, 0.0f},  // 54
        // {1.5f, -0.5f,  0.5f,  1.0f, 0.0f},  // 55
        // {0.5f, -0.5f,  0.5f,  1.0f, 1.0f},  // 56
        // {0.5f, -0.5f,  0.5f,  1.0f, 1.0f},  // 57
        // {0.5f, -0.5f, -0.5f,  0.0f, 1.0f},  // 58
        // {1.5f, -0.5f, -0.5f,  0.0f, 0.0f},  // 59

        // {1.5f,  0.5f, -0.5f,  0.0f, 0.0f},  // 60
        // {1.5f,  0.5f,  0.5f,  1.0f, 0.0f},  // 61
        // {0.5f,  0.5f,  0.5f,  1.0f, 1.0f},  // 62
        // {0.5f,  0.5f,  0.5f,  1.0f, 1.0f},  // 63
        // {0.5f,  0.5f, -0.5f,  0.0f, 1.0f},  // 64
        // {1.5f,  0.5f, -0.5f,  0.0f, 0.0f},  // 65
    
        },
        
        //Index
        //Cube 1
        {0, 1, 2, 3, 4, 5,     // Top face
        6, 7, 8, 9, 10, 11,   // Bottom face
        12, 13, 14, 15, 16, 17, // Front face
        18, 19, 20, 21, 22, 23, // Left face
        24, 25, 26, 27, 28, 29, // Back face
        30, 31, 32, 33, 34, 35} // Right face

    };
    
    GLuint mesh1 = registerMesh(raw_mesh);
    // GLuint mesh2 = registerMesh(raw_mesh);
    // GLuint mesh3 = registerMesh(raw_mesh);
    // GLuint mesh4 = registerMesh(raw_mesh);
    // GLuint mesh5 = registerMesh(raw_mesh);
    // GLuint mesh6 = registerMesh(raw_mesh);
    // GLuint mesh7 = registerMesh(raw_mesh);
    // GLuint mesh8 = registerMesh(raw_mesh);


    // //TEXTURE
    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // // set the texture wrapping/filtering options (on the currently bound texture object)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load and generate the texture
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("../resource/cube1.jpg", &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);

    // GLuint textureLocation = glGetUniformLocation(shaderProgram, "ourTexture");
    // glUniform1i(textureLocation, 0); // Bind the texture to texture unit 0

    //TRANSFORM
    // unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); //new

    // Load Texture
    GLuint texture0 = loadTexture("../resource/cube0.jpg", shaderProgram);
    GLuint texture1 = loadTexture("../resource/cube1.jpg", shaderProgram);
    GLuint texture2 = loadTexture("../resource/cube2.jpg", shaderProgram);
    GLuint texture3 = loadTexture("../resource/cube3.jpg", shaderProgram);
    GLuint texture4 = loadTexture("../resource/cube4.jpg", shaderProgram);
    GLuint texture5 = loadTexture("../resource/cube5.jpg", shaderProgram);
    GLuint texture6 = loadTexture("../resource/cube6.jpg", shaderProgram);
    GLuint texture7 = loadTexture("../resource/cube7.jpg", shaderProgram);

    cubeData.push_back({posFromState[StatePos::pos_0], StatePos::pos_0, texture0});
    cubeData.push_back({posFromState[StatePos::pos_1], StatePos::pos_1, texture1});
    cubeData.push_back({posFromState[StatePos::pos_2], StatePos::pos_2, texture2});
    cubeData.push_back({posFromState[StatePos::pos_3], StatePos::pos_3, texture3});
    cubeData.push_back({posFromState[StatePos::pos_4], StatePos::pos_4, texture4});
    cubeData.push_back({posFromState[StatePos::pos_5], StatePos::pos_5, texture5});
    cubeData.push_back({posFromState[StatePos::pos_6], StatePos::pos_6, texture6});
    cubeData.push_back({posFromState[StatePos::pos_7], StatePos::pos_7, texture7});

    //Main Event Loop
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        selectShaderProgram(shaderProgram);

        for (auto cube : cubeData) {
            renderCube(cube, cameraPos, shaderProgram);
            drawMesh(mesh1, raw_mesh.getNumIndicies());
        }

        // renderCube(glm::vec3(-0.5f, 0.5f, 0.5f), glm::radians(0.0f), cameraPos, shaderProgram);   // Top face, left cubelet
        // drawMesh(mesh1, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(0.5f, 0.5f, 0.5f), glm::radians(90.0f), cameraPos, shaderProgram);    // Top face, right cubelet
        // drawMesh(mesh2, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(-0.5f, -0.5f, 0.5f), glm::radians(180.0f), cameraPos, shaderProgram); // Bottom face, left cubelet
        // drawMesh(mesh3, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(0.5f, -0.5f, 0.5f), glm::radians(270.0f), cameraPos, shaderProgram);  // Bottom face, right cubelet
        // drawMesh(mesh4, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(-0.5f, 0.5f, -0.5f), glm::radians(0.0f), cameraPos, shaderProgram);   // Top-left
        // drawMesh(mesh5, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(0.5f, 0.5f, -0.5f), glm::radians(90.0f), cameraPos, shaderProgram);    // Top-right
        // drawMesh(mesh6, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(-0.5f, -0.5f, -0.5f), glm::radians(180.0f), cameraPos, shaderProgram); // Bottom-left
        // drawMesh(mesh7, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(0.5f, -0.5f, -0.5f), glm::radians(270.0f), cameraPos, shaderProgram);  // Bottom-right
        // drawMesh(mesh8, raw_mesh.getNumIndicies());
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "Window Closed" << std::endl;
}