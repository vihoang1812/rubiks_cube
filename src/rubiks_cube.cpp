#include "rubiks_cube.h"
#include "event_handler.h"
#include "texture.h"
#include "graphics.h"
#include "constants.h"

static Graphics& g = Graphics::getInstance();

RubiksCube::RubiksCube() {
    EventHandler::getInstance().subscribe(this);

    Texture texture0 = Texture("../resource/cube0.jpg");
    Texture texture1 = Texture("../resource/cube1.jpg");
    Texture texture2 = Texture("../resource/cube2.jpg");
    Texture texture3 = Texture("../resource/cube3.jpg");
    Texture texture4 = Texture("../resource/cube4.jpg");
    Texture texture5 = Texture("../resource/cube5.jpg");
    Texture texture6 = Texture("../resource/cube6.jpg");
    Texture texture7 = Texture("../resource/cube7.jpg");

    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_0], StatePos::pos_0, texture0.getId()});
    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_1], StatePos::pos_1, texture1.getId()});
    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_2], StatePos::pos_2, texture2.getId()});
    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_3], StatePos::pos_3, texture3.getId()});
    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_4], StatePos::pos_4, texture4.getId()});
    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_5], StatePos::pos_5, texture5.getId()});
    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_6], StatePos::pos_6, texture6.getId()});
    cubeData.push_back({PosFromState::getInstance()[StatePos::pos_7], StatePos::pos_7, texture7.getId()});
}

void RubiksCube::draw() {
    for(auto cube : cubeData) {
        renderCube(cube);
        g.drawMesh(cube.getMesh().getId(), cube.getMesh().getNumIndicies());
    }
}

void RubiksCube::renderCube(Cube& cube) {
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    
    projection = glm::perspective(glm::radians(45.0f), (float) Constants::SCR_WIDTH / (float) Constants::SCR_HEIGHT, 0.1f, 100.0f);

    view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());

    view = glm::rotate(view, camera.rotX(), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, camera.rotY(), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, camera.rotZ(), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 objPos = cube.curPos;

    model = glm::translate(model, objPos);

    model = model * glm::mat4_cast(cube.rot);
    
    g.setUniformMat4fv("model", glm::value_ptr(model));
    g.setUniformMat4fv("view", &view[0][0]);
    g.setUniformMat4fv("projection", &projection[0][0]);

    g.setTexture(cube.textureID);
}

void RubiksCube::onEvent(Event e) {
    
    if (e == Event::RUBIK_RESET) {
        for(auto& cube : cubeData) {
            cube.reset();
        }
    }

    if (e == Event::RUBIK_ROTATE_RIGHT) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_7) {
                rotate_cube_right(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_RIGHT_INV) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_7) {
                rotate_cube_right_inv(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_LEFT) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_1 || cube.getState() == StatePos::pos_2 || 
                cube.getState() == StatePos::pos_5 || cube.getState() == StatePos::pos_6) {
                rotate_cube_left(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_LEFT_INV) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_1 || cube.getState() == StatePos::pos_2 || 
                cube.getState() == StatePos::pos_5 || cube.getState() == StatePos::pos_6) {
                rotate_cube_left_inv(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_UP) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3) {
                rotate_cube_up(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_UP_INV) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3) {
                rotate_cube_up_inv(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_BOTTOM) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_bot(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_BOTTOM_INV) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_bot_inv(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_FRONT) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_front(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_FRONT_INV) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_2 || cube.getState() == StatePos::pos_3 || 
                cube.getState() == StatePos::pos_6 || cube.getState() == StatePos::pos_7) {
                rotate_cube_front_inv(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_BACK) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5) {
                rotate_cube_back(cube);
            }
        }
    }

    if (e == Event::RUBIK_ROTATE_BACK_INV) {
        for(auto& cube : cubeData) {
            if (cube.getState() == StatePos::pos_0 || cube.getState() == StatePos::pos_1 || 
                cube.getState() == StatePos::pos_4 || cube.getState() == StatePos::pos_5) {
                rotate_cube_back_inv(cube);
            }
        }
    }

}