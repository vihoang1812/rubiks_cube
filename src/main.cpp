#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GL_PIXEL_SHADER GL_FRAGMENT_SHADER

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//glm::vec3 camera = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);    
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float camRot = 0;
float cubeRotAng = 0.0f;

//double rad(double deg) {
//    return deg * (M_PI / 180.0);
//}

static void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float cameraSpeed = 0.5f; // adjust accordingly

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        std::cout << "Enter was Pressed :)" << std::endl; 
        cameraPos += glm::vec3(0.1f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::cout << "Down was Pressed :)" << std::endl; 
        camRot += 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::cout << "Up was Pressed :)" << std::endl; 
        camRot -= 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        std::cout << "RIGHT_SHIFT was Pressed :)" << std::endl; 
        cameraPos += cameraSpeed * glm::vec3(0.0f, 0.0f, 0.2f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        std::cout << "LEFT_SHIFT was Pressed :)" << std::endl; 
        cameraPos += cameraSpeed * glm::vec3(0.0f, 0.0f, -0.2f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        std::cout << "Rotate left" << std::endl;
        cubeRotAng += glm::radians(45.0f); 
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        std::cout << "Rotate right" << std::endl;
        cubeRotAng -= glm::radians(45.0f); 
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

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

void renderCube(glm::vec3 objPos,float initAngle, glm::vec3 camPos, GLuint shaderProgram) {
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    //view  = glm::translate(view, cameraPos);
    view = glm::rotate(view, camRot, glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::translate(model, objPos);

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
    float r = sqrt(2.0f)/2;
    float z = r * cos(cubeRotAng + initAngle);
    float x = r * sin(cubeRotAng + initAngle);

    float rotAngle = 45.0f;

    float x_new = x * cos(glm::radians(rotAngle)) + z * sin(glm::radians(rotAngle));
    float z_new = -x * sin(glm::radians(rotAngle)) + z * cos(glm::radians(rotAngle));

    model = glm::translate(model, glm::vec3(x_new, 0.0f, z_new));
    model = glm::rotate(model, cubeRotAng + initAngle , glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis

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
}

void drawMesh(GLuint drawableId, GLuint totalIndicies) {
    if(drawableId == 0) {
        throw new std::runtime_error("Drawable id is invalid");
    }

    glBindVertexArray(drawableId);
    glDrawElements(GL_TRIANGLES, totalIndicies, GL_UNSIGNED_INT, 0);
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
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
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
        {-0.5f, -0.5f,  0.5f,  0.5f, 0.5f},
        { 0.5f, -0.5f,  0.5f,  0.75f,  0.5f},
        { 0.5f,  0.5f,  0.5f,  0.75f,  0.25f},
        { 0.5f,  0.5f,  0.5f,  0.75f,  0.25f},
        {-0.5f,  0.5f,  0.5f,  0.5f, 0.25f},
        {-0.5f, -0.5f,  0.5f,  0.5f, 0.5f},

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
        {-0.5f,  0.5f, -0.5f,  0.25f,  0.5f},
        {-0.5f,  0.5f,  0.5f,  0.5f, 0.5f},
        {-0.5f, -0.5f,  0.5f,  0.5f, 0.25f},
        {-0.5f, -0.5f,  0.5f,  0.5f, 0.25f},
        {-0.5f, -0.5f, -0.5f,  0.25f,  0.25f},
        {-0.5f,  0.5f, -0.5f,  0.25f,  0.5f},

        // back face (square 7)
        {-0.5f,  0.5f,  0.5f, 0.5f,   0.25f}, 
        { 0.5f,  0.5f,  0.5f, 0.75f,  0.25f}, 
        { 0.5f,  0.5f, -0.5f, 0.75f,  0.0f}, 
        { 0.5f,  0.5f, -0.5f, 0.75f,  0.0f}, 
        {-0.5f,  0.5f, -0.5f, 0.5f,   0.0f}, 
        {-0.5f,  0.5f,  0.5f, 0.5f,   0.25f}, 

        // Right black (square 8)
        { 0.5f,  0.5f,  0.5f,  0.75f,  0.5f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 0.5f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 0.25f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 0.25f},
        { 0.5f, -0.5f,  0.5f,  0.75f,  0.25f},
        { 0.5f,  0.5f,  0.5f,  0.75f,  0.5f}

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

        //Cube 2
        // 36, 37, 38, 39, 40, 41,
        // 42, 43, 44, 45, 46, 47, 
        // 48, 49, 50, 51, 52, 53,
        // 54, 55, 56, 57, 58, 59, 
        // 60, 61, 62, 63, 64, 65
        //66, 67, 68, 69, 70, 71

        // 72, 73, 74, 75, 76, 77
        // 78, 79, 80, 81, 82, 83 
        // 84, 85, 86, 87, 88, 89, 
        // 90, 91, 92, 93, 94, 95,
        // 96, 97, 98, 99, 100, 101
        //}
    };
    
    GLuint mesh1 = registerMesh(raw_mesh);
    GLuint mesh2 = registerMesh(raw_mesh);
    GLuint mesh3 = registerMesh(raw_mesh);
    GLuint mesh4 = registerMesh(raw_mesh);
    GLuint mesh5 = registerMesh(raw_mesh);
    GLuint mesh6 = registerMesh(raw_mesh);
    GLuint mesh7 = registerMesh(raw_mesh);
    GLuint mesh8 = registerMesh(raw_mesh);

    //TEXTURE
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../resource/cube1.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    GLuint textureLocation = glGetUniformLocation(shaderProgram, "ourTexture");
    glUniform1i(textureLocation, 0); // Bind the texture to texture unit 0

    //TRANSFORM
    // unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); //new

    //Main Event Loop
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        selectShaderProgram(shaderProgram);
        
        renderCube(glm::vec3(-0.5f, 0.0f, 0.0f), glm::radians(0.0f), cameraPos, shaderProgram);
        drawMesh(mesh1, raw_mesh.getNumIndicies());

        renderCube(glm::vec3(0.5f, 0.0f, 0.0f), glm::radians(90.0f), cameraPos, shaderProgram);
        drawMesh(mesh2, raw_mesh.getNumIndicies());

        renderCube(glm::vec3(-0.5f, -1.0f, 0.0f), glm::radians(180.0f), cameraPos, shaderProgram);
        drawMesh(mesh3, raw_mesh.getNumIndicies());

        renderCube(glm::vec3(0.5f, -1.0f, 0.0f), glm::radians(270.0f), cameraPos, shaderProgram);
        drawMesh(mesh4, raw_mesh.getNumIndicies());

        // renderCube(glm::vec3(-0.5f, 0.0f, -1.0f), camera, shaderProgram);
        // drawMesh(mesh5, raw_mesh.getNumIndicies());
        // renderCube(glm::vec3(0.5f, 0.0f, -1.0f), camera, shaderProgram);
        // drawMesh(mesh6, raw_mesh.getNumIndicies());
        // renderCube(glm::vec3(-0.5f, -1.0f, -1.0f), camera, shaderProgram);
        // drawMesh(mesh7, raw_mesh.getNumIndicies());
        // renderCube(glm::vec3(0.5f, -1.0f, -1.0f), camera, shaderProgram);
        // drawMesh(mesh8, raw_mesh.getNumIndicies());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "Window Closed" << std::endl;
}