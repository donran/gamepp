#pragma once
#include "../../stb/image.hpp"
#include <GL/gl.h>

namespace enginepp::graphics::textures {
class GLTexture {
  private:
    unsigned int id = -1;

  public:
    GLTexture() {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Bind() {
        glBindTexture(GL_TEXTURE_2D, id);
    }
    void Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    int SetTexture(stb::Image *img);
};
} // namespace enginepp::graphics::textures
