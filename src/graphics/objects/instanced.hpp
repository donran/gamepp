#pragma once
#include "vao.hpp"

namespace enginepp::graphics::objects {
#define _vertexCallbackFuncPtr std::function<void(VBO &)>
class InstancedBuffer {
  private:
    VAO m_vertexArray;
    VBO m_staticVertexBuffer;
    VBO m_instancedVertexBuffer;

  public:
    InstancedBuffer() {
    }

    ~InstancedBuffer() {
    }

    void StaticVBO(_vertexCallbackFuncPtr);
    void InstancedVBO(_vertexCallbackFuncPtr);
    // GLenum mode, GLint first, GLsizei count, GLsizei primcount
    void DrawArrays(GLenum mode, GLint first, GLsizei count, GLsizei primcount) {
        m_vertexArray.Bind();
        glDrawArraysInstanced(mode, first, count, primcount);
        m_vertexArray.Unbind();
    }

    void Bind() {
        m_vertexArray.Bind();
    }

    void Unbind() {
        m_vertexArray.Bind();
    }

    void Enable() {
        m_vertexArray.Bind();
        m_vertexArray.EnableVertexBufferObjects({m_staticVertexBuffer, m_instancedVertexBuffer});
    }
};
}; // namespace enginepp::graphics::objects
