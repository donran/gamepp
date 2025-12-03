#pragma once
#include <glm/glm.hpp>
namespace enginepp::graphics::objects {
// struct [[gnu::packed]] Rectangle {
struct Rectangle {
    glm::vec2 position;
    glm::vec4 color;
    unsigned int spriteId;
};
} // namespace enginepp::graphics::objects
