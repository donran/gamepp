#include "rect.hpp"
#pragma
#include "instanced.hpp"

namespace enginepp::graphics::objects {

class SpriteBuffer {
  private:
    InstancedBuffer m_instancedBuffer;
    float m_quadVertices[4 * 6] = {
        -0.5f, 0.5f,  0.0f, 1.0f, // Top left
        -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
        0.5f,  -0.5f, 1.0f, 0.0f, // Bottom Right

        -0.5f, 0.5f,  0.0f, 1.0f, // Top left
        0.5f,  -0.5f, 1.0f, 0.0f, // Bottom right
        0.5f,  0.5f,  1.0f, 1.0f  // Top right
    };
    bool m_initialized = false;

  public:
    SpriteBuffer() {
        m_instancedBuffer.StaticVBO([this](VBO &buffer) {
            buffer.Attributes([](struct VertexBufferAttributes *attrs) {
                attrs->Add(0, GL_FLOAT, 2); // Position
                attrs->Add(1, GL_FLOAT, 2); // uv/texcoord
            });
            buffer.BufferData(sizeof(m_quadVertices) / 4, m_quadVertices);
        });
        m_instancedBuffer.InstancedVBO([](VBO &buffer) {
            buffer.Attributes([](struct VertexBufferAttributes *attrs) {
                attrs->Add(2, GL_FLOAT, 2, 1); // Position
                attrs->Add(3, GL_FLOAT, 4, 1); // Color
                attrs->Add(4, GL_INT, 1, 1);   // SpriteID
            });
        });
        m_instancedBuffer.Enable();
    }

    ~SpriteBuffer() {
    }

    void Buffer(std::vector<Rectangle> &rectinfos) {
        // Should this distinction be in the buffer itself?
        if (!m_initialized) {
            m_instancedBuffer.InstancedVBO([&rectinfos](VBO &buffer) {
                buffer.BufferData(rectinfos.size(), &rectinfos[0], GL_DYNAMIC_DRAW);
            });
        } else {
            m_instancedBuffer.InstancedVBO([&rectinfos](VBO &buffer) {
                buffer.UpdateData(rectinfos.size(), &rectinfos[0]);
            });
        }
    }

    void Draw(GLsizei count) {
        m_instancedBuffer.DrawArrays(GL_TRIANGLES, 0, sizeof(m_quadVertices) / 4, count);
    }
};

}; // namespace enginepp::graphics::objects
