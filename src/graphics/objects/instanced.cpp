#include "instanced.hpp"

using namespace enginepp::graphics::objects;

void InstancedBuffer::StaticVBO(_vertexCallbackFuncPtr cbfunk) {
    m_vertexArray.Bind();
    cbfunk(m_staticVertexBuffer);
}

void InstancedBuffer::InstancedVBO(_vertexCallbackFuncPtr cbfunk) {
    m_vertexArray.Bind();
    cbfunk(m_instancedVertexBuffer);
}
