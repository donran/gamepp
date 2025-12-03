#include "image.hpp"
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include "../../libs/stb_image.h"

enginepp::stb::Image::Image(const char *file_path) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;

    unsigned char *data = stbi_load(file_path, &width, &height, &nrChannels, 0);

    if (data == 0) {
        this->failed = true;
        stbi_image_free(data);
        return;
    }

    this->width = width;
    this->height = height;
    this->channels = nrChannels;
    this->data = data;
}

enginepp::stb::Image::~Image() {
    if (this->Ok()) {
        stbi_image_free(this->data);
    }
}
