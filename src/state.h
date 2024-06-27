#ifndef STATE_POS_H
#define STATE_POS_H

#include <glm/glm.hpp>

class Cube; //To avoid cyclic dependancies

//This probably needs to be moved to a utility folder / class

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

class PosFromState {
private:
    PosFromState() = default;

public:
    glm::vec3 operator[](StatePos pos);
    static PosFromState& getInstance();
};

void rotate_cube_right(Cube& cube);
void rotate_cube_right_inv(Cube& cube);
void rotate_cube_left(Cube& cube);
void rotate_cube_left_inv(Cube& cube);
void rotate_cube_up(Cube& cube);
void rotate_cube_up_inv(Cube& cube);
void rotate_cube_bot(Cube& cube);
void rotate_cube_bot_inv(Cube& cube);
void rotate_cube_front(Cube& cube);
void rotate_cube_front_inv(Cube& cube);
void rotate_cube_back(Cube& cube);
void rotate_cube_back_inv(Cube& cube);

#endif