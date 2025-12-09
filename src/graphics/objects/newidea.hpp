#pragma once
#include "instanced.hpp"
#include "renderable.hpp"
#include "vbo.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace enginepp::graphics::objects::test {

static float _QuadVertices[] = {
    -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom Right

    -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom right
    0.5f,  0.5f,  1.0f, 1.0f  // Top right
};

struct Emptiness {
    float *StaticVertices() {
        return NULL;
    }
    void AddStaticAttributes(struct VertexBufferAttributes *attrs) {
    }
    void AddInstancedAttributes(struct VertexBufferAttributes *attrs) {
    }
};
// TODO: One question to ask if we should just allow these objects to
// use a incrementing attribute index
struct Quad : public Renderable {
    float *StaticVertices() {
        return _QuadVertices;
    }
    unsigned int VerticesCount() override {
        return sizeof(_QuadVertices) / sizeof(float) / 4;
    }
    float *Vertices() override {
        return _QuadVertices;
    }
    void AddStaticAttributes(struct VertexBufferAttributes *attrs) override {
        attrs->Add(0, GL_FLOAT, 2); // Position
        attrs->Add(1, GL_FLOAT, 2); // uv/texcoord
    }
};

struct Object2D : Quad {
    glm::mat4 model;
    void AddInstancedAttributes(struct VertexBufferAttributes *attrs) override {
        Quad::AddInstancedAttributes(attrs);
        attrs->Add4(2, GL_FLOAT, 4, 1);
    }
};

struct TexturedObject : Object2D {
    unsigned int spriteId;
    void AddInstancedAttributes(struct VertexBufferAttributes *attrs) override {
        Object2D::AddInstancedAttributes(attrs);
        attrs->Add(6, GL_INT, 1, 1); // SpriteID
    }
};

class RenderableGameObject {
  public:
    virtual operator Renderable &() = 0;
};

class GameObject2D {
  private:
    TexturedObject obj;
    glm::vec2 m_position;

  public:
    operator Renderable &() {
        return obj;
    };

    const glm::vec2 &Position() const {
        return m_position;
    }

    void Position(const glm::vec2 pos) {
        m_position = pos;
    }

    void SetupAttributes(InstancedBuffer &ibuffer) {
        ibuffer.StaticVBO([this](VBO &buffer) {
            buffer.Attributes([this](struct VertexBufferAttributes *attrs) {
                obj.AddStaticAttributes(attrs);
            });
        });
        ibuffer.InstancedVBO([this](VBO &buffer) {
            buffer.Attributes([this](struct VertexBufferAttributes *attrs) {
                obj.AddInstancedAttributes(attrs);
            });
        });
    }
};

#include <type_traits>

template <typename T>
concept RenderableConcept = std::is_base_of_v<T, GameObject2D>;

template <RenderableConcept T>
class RenderableBuffer {
    InstancedBuffer m_instancedBuffer;
    bool m_initialized = false;

    unsigned int m_vertCount = 0;

  public:
    RenderableBuffer() {
        T tmp;
        m_vertCount = tmp.VerticesCount();
        tmp.SetupAttributes(m_instancedBuffer);
    }
    void Buffer(std::vector<RenderableGameObject> objs) {
        m_instancedBuffer.InstancedVBO([&objs, this](VBO &buffer) {
            if (!m_initialized) {
                buffer.BufferData(objs.size(), &objs[0], GL_DYNAMIC_DRAW);
            } else {
                buffer.UpdateData(objs.size(), &objs[0]);
            }
        });
    }
    void Draw(GLsizei count) {
        m_instancedBuffer.DrawArrays(GL_TRIANGLES, 0, m_vertCount, count);
    }
    void Bind() {
        m_instancedBuffer.Bind();
    }

    void Unbind() {
        m_instancedBuffer.Bind();
    }
};

}; // namespace enginepp::graphics::objects::test
