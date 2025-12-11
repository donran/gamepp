#include "newidea2.hpp"
#include <cstring>

using namespace enginepp::graphics::objects::test;

struct Rectangle *NewRectangle() {
    auto res = new struct Rectangle;
    memcpy(res->vertices, _QuadVertices, sizeof(_QuadVertices));
    return res;
    // res->vertices = {
    //     -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    //     -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
    //     0.5f,  -0.5f, 1.0f, 0.0f, // Bottom Right

    //     -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    //     0.5f,  -0.5f, 1.0f, 0.0f, // Bottom right
    //     0.5f,  0.5f,  1.0f, 1.0f  // Top right
    // };
}
