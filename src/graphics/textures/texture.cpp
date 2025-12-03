#include "texture.hpp"

int enginepp::graphics::textures::GLTexture::SetTexture(stb::Image *img) {
    if (this->id == -1) {
        return -1;
    }
    if (!img->Ok()) {
        return -1;
    }
    this->Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->Width(), img->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img->Data());
    this->Unbind();

    return 0;
}
