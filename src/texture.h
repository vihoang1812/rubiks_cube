#ifndef TEXTURE_H
#define TEXTURE_H

#include "common_alias.h"

class Texture {
private:
    uint id;
public:
    Texture(str fileName);
    uint getId();
};

#endif
