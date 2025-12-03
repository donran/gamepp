#pragma once

struct stb_image {};

namespace enginepp::stb {
class Image {
  private:
    int width;
    int height;
    int channels;
    unsigned char *data;
    bool failed = false;

  public:
    Image(const char *file_path);
    ~Image();
    int Width() {
        return this->width;
    }
    int Height() {
        return this->height;
    }
    int Channels() {
        return this->channels;
    }
    unsigned char *Data() {
        return this->data;
    }

    bool Ok() {
        return !this->failed;
    }
};
// struct stb_image *stblib_load_image(const char *file_path);
} // namespace enginepp::stb
