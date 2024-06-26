#include "texture.h"
#include "graphics.h"

Texture::Texture(str fileName) {
    id = Graphics::getInstance().registerTexture(fileName);
}

uint Texture::getId() {
    return id;
}