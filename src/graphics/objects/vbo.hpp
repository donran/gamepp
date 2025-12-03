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

    void Add(unsigned int index, GLenum type, GLsizei size, unsigned int divisor = 0) {
        attributes.push_back((struct VertexAttribute){
            .index = index,
            .size = size,
            .type = type,
            .divisor = divisor,
        });
    }
};

class VBO {
  private:
    unsigned int m_id;
    struct VertexBufferAttributes m_attributes;

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

    void SetAttribute(unsigned int index, int size, GLenum type, GLboolean normalized,
                      GLsizei stride = -1, const GLvoid *ptr = (void *)0);
    void SetAttributeI(unsigned int index, int size, GLenum type, GLsizei stride = -1,
                       const GLvoid *ptr = (void *)0);

    void SetDivisor(unsigned int index, unsigned int divisor) {
        glVertexAttribDivisor(index, divisor);
    }

    void BufferData(GLsizeiptr size, const GLvoid *data, GLenum usage = GL_STATIC_DRAW) {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        // Unbind();
    }

    void UpdateData(GLsizeiptr size, const GLvoid *data, GLintptr offset = 0) {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        Unbind();
    }

    struct VertexBufferAttributes *Attributes(size_t count) {
        m_attributes.attributes.clear();
        m_attributes.attributes.resize(0);
        return &m_attributes;

        // bool is_glm_vec = std::is_base_of_v<glm::vec1, T> || std::is_base_of_v<glm::vec2, T> ||
        //                   std::is_base_of_v<glm::vec3, T> || std::is_base_of_v<glm::vec4, T>;
        // std::cout << "hello: " << std::is_array<T>::value << std::endl;
        // if (is_glm_vec) {
        //     typ = GL_FLOAT;
        // }
    }
    void Attributes2(std::function<void((struct VertexBufferAttributes *))> attrfunc);

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
