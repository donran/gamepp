
#pragma once
#include "vbo.hpp"
#include <GL/glew.h>
#include <initializer_list>
namespace enginepp::graphics::objects {
class VAO {
  private:
    unsigned int m_id;

  public:
    VAO() {
        glGenVertexArrays(1, &m_id);
    }
    ~VAO() {
        glDeleteVertexArrays(1, &m_id);
    }
    void Bind() {
        glBindVertexArray(m_id);
    }
    void Unbind() {
        glBindVertexArray(0);
    }

    void EnableVertexArrayAttribute(unsigned int index) {
        glEnableVertexArrayAttrib(m_id, index);
    }

    void EnableVertexBufferObjects(std::initializer_list<VBO> list) {
        for (auto el : list) {
            for (auto attr : el.GetAttributes()->attributes) {
                std::cout << "enabling id " << attr.index << std::endl;
                EnableVertexArrayAttribute(attr.index);
                if (attr.divisor != 0) {
                    glVertexAttribDivisor(attr.index, attr.divisor);
                }
            }
        }
    }
};
} // namespace enginepp::graphics::objects
