#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "common_alias.h"

struct Constants {
    static constexpr uint SCR_WIDTH = 800;
    static constexpr uint SCR_HEIGHT = 600;
    static constexpr uint OPENGL_MAJOR_VERSION = 4;
    static constexpr uint OPENGL_MINOR_VERSION = 1;
    static constexpr auto SRC_TITLE = "Vi's Rubiks Cube";
    static constexpr uint OPENGL_SWAP_INTERVAL = 1;
    static constexpr std::tuple<float, float, float, float> SCR_BACKGROUND_COLOUR = {0.2f, 0.3f, 0.6f, 1.0f};
};

#endif
