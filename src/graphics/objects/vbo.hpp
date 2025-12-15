#pragma once
#include <GL/glew.h>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

static unsigned int _type_to_size(GLenum type) {
    switch (type) {
    case GL_BYTE:
        return 1;
    case GL_SHORT:
        return 2;
    case GL_FLOAT:
    case GL_INT:
        return 4;
    }
    // TODO: Handle this lmaoooo
    return 1;
}
namespace enginepp::graphics::objects {

struct VertexAttribute {
    unsigned int index;
    GLsizei size;
    GLenum type;
    unsigned int divisor;
};

struct VertexBufferAttributes {
    std::vector<VertexAttribute> attributes;
    size_t size;

    void Add(unsigned int index, GLenum type, GLsizei size, unsigned int divisor = 0) {
        attributes.push_back((struct VertexAttribute){
            .index = index,
            .size = size,
            .type = type,
            .divisor = divisor,
        });
    }
    void Add4(unsigned int index, GLenum type, GLsizei size, unsigned int divisor = 0) {
        // attrs->Add(2, GL_FLOAT, 4, 1); // Model
        // attrs->Add(3, GL_FLOAT, 4, 1); // Model
        // attrs->Add(4, GL_FLOAT, 4, 1); // Model
        // attrs->Add(5, GL_FLOAT, 4, 1); // Model
        for (unsigned int i = 0; i < 4; i++) {
            attributes.push_back((struct VertexAttribute){
                .index = index + i,
                .size = size,
                .type = type,
                .divisor = divisor,
            });
        }
    }
};

class VBO {
  protected:
    unsigned int m_id;
    struct VertexBufferAttributes m_attributes = {};

  public:
    VBO() {
        glGenBuffers(1, &m_id);
    }
    void Bind() {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }
    void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetAttribute(unsigned int index, int size, GLenum type, GLboolean normalized, GLsizei stride = -1,
                      const GLvoid *ptr = (void *)0);
    void SetAttributeI(unsigned int index, int size, GLenum type, GLsizei stride = -1, const GLvoid *ptr = (void *)0);

    void SetDivisor(unsigned int index, unsigned int divisor) {
        glVertexAttribDivisor(index, divisor);
    }

    void BufferData(GLsizeiptr count, const GLvoid *data, GLenum usage = GL_STATIC_DRAW) {
        // TODO: uh should we warn if attributes aren't created yet?
        Bind();
        std::cout << "sizer: " << count * m_attributes.size << std::endl;
        std::cout << "count: " << count << ", size: " << m_attributes.size << std::endl;
        glBufferData(GL_ARRAY_BUFFER, count * m_attributes.size, data, usage);
        // Unbind();
    }

    void UpdateData(GLsizeiptr count, const GLvoid *data, GLintptr offset = 0) {
        // TODO: uh should we warn if attributes aren't created yet?
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, count * m_attributes.size, data);
        // Unbind();
    }

    void Attributes(std::function<void((struct VertexBufferAttributes *))> attrfunc);

    std::vector<unsigned int> AttributeIndices() {
        auto res = std::vector<unsigned int>();
        for (auto &attr : m_attributes.attributes) {
            res.push_back(attr.index);
        }
        return res;
    }

    struct VertexBufferAttributes *GetAttributes() {
        return &m_attributes;
    }
};

} // namespace enginepp::graphics::objects
