#include "vbo.hpp"
using namespace enginepp::graphics::objects;

void VBO::SetAttribute(unsigned int index, int size, GLenum type, GLboolean normalized,
                       GLsizei stride, const GLvoid *ptr) {
    if (stride == -1) {
        stride = size * sizeof(type);
    }

    // glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glVertexAttribPointer(index, size, type, normalized, stride, ptr);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::SetAttributeI(unsigned int index, int size, GLenum type, GLsizei stride,
                        const GLvoid *ptr) {
    if (stride == -1) {
        stride = size * sizeof(type);
    }

    // glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glVertexAttribIPointer(index, size, type, stride, ptr);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::Attributes(std::function<void((struct VertexBufferAttributes *))> attrFunc) {
    m_attributes.attributes.clear();
    m_attributes.attributes.resize(0);
    attrFunc(&m_attributes);
    std::sort(m_attributes.attributes.begin(), m_attributes.attributes.end(),
              [](VertexAttribute &a, VertexAttribute &b) { return a.index < b.index; });
    unsigned int stride = 0;
    unsigned long offset = 0;
    for (auto &attr : m_attributes.attributes) {
        stride += attr.size * _type_to_size(attr.type);
    }
    Bind();
    for (auto &attr : m_attributes.attributes) {
        if (attr.type == GL_INT) {
            glVertexAttribIPointer(attr.index, attr.size, attr.type, stride, (void *)offset);
        } else {
            glVertexAttribPointer(attr.index, attr.size, attr.type, GL_FALSE, stride,
                                  (void *)offset);
        }
        /// ????????????????
        offset += ((unsigned long)attr.size * _type_to_size(attr.type));
    }
    m_attributes.size = stride;

    // Unbind();
}
