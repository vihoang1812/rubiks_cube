#ifndef STATE_POS_H
#define STATE_POS_H

#include <glm/glm.hpp>

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

    static PosFromState& getInstance() {
        static PosFromState instance;
        return instance;
    }
};

#endif