#include "state.h"
#include "cube.h"

glm::vec3 PosFromState::operator[](StatePos pos) {
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

PosFromState& PosFromState::getInstance() {
    static PosFromState instance;
    return instance;
}

void rotate_cube_right(Cube& cube) {
    cube.rotateX(-90.0f);
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_3);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_right_inv(Cube& cube) {
    cube.rotateX(90.0f);
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_4);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_left(Cube& cube) {
    cube.rotateX(-90.0f);
    switch(cube.curState) {
        case StatePos::pos_1:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_2);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_left_inv(Cube& cube) {
    cube.rotateX(90.0f);
    switch(cube.curState) {
        case StatePos::pos_1:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_2:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_5);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_up(Cube& cube) {
    cube.rotateZ(-90.0f);
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
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_up_inv(Cube& cube) {
    cube.rotateZ(90.0f);
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
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_bot(Cube& cube) {
    cube.rotateZ(-90.0f);
    switch(cube.curState) {
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
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_bot_inv(Cube& cube) {
    cube.rotateZ(90.0f);
    switch(cube.curState) {
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
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_front(Cube& cube) {
    cube.rotateY(-90.0f);
    switch(cube.curState) {
        case StatePos::pos_2:
            cube.setState(StatePos::pos_6);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_3);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_front_inv(Cube& cube) {
    cube.rotateY(90.0f);
    switch(cube.curState) {
        case StatePos::pos_2:
            cube.setState(StatePos::pos_3);
            break;
        case StatePos::pos_3:
            cube.setState(StatePos::pos_7);
            break;
        case StatePos::pos_6:
            cube.setState(StatePos::pos_2);
            break;
        case StatePos::pos_7:
            cube.setState(StatePos::pos_6);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_back(Cube& cube) {
    cube.rotateY(-90.0f);
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_1);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_4);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}

void rotate_cube_back_inv(Cube& cube) {
    cube.rotateY(90.0f);
    switch(cube.curState) {
        case StatePos::pos_0:
            cube.setState(StatePos::pos_4);
            break;
        case StatePos::pos_1:
            cube.setState(StatePos::pos_0);
            break;
        case StatePos::pos_4:
            cube.setState(StatePos::pos_5);
            break;
        case StatePos::pos_5:
            cube.setState(StatePos::pos_1);
            break;
        default:
            throw std::runtime_error("undefined state");
    }
}
