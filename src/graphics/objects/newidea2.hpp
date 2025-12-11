#pragma once
#include "renderable.hpp"
#include "vbo.hpp"

static float _QuadVertices[] = {
    -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom Right

    -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom right
    0.5f,  0.5f,  1.0f, 1.0f  // Top right
};
static size_t _QuadVerticesSize = 4;

namespace enginepp::graphics::objects::test {
struct RenderableTwo {
    virtual float *Vertices() = 0;
    virtual unsigned int VertexCount() = 0;
    virtual unsigned int VertexSize() = 0;
};

struct ShaderAttributes {
    virtual void Attributes(struct VertexBufferAttributes *attrs) = 0;
};

struct Quad : ShaderAttributes {
    void Attributes(struct VertexBufferAttributes *attrs) {
        attrs->Add(0, GL_FLOAT, 2); // Position
        attrs->Add(1, GL_FLOAT, 2); // uv/texcoord
    }
};

struct Rectangle : Quad, public RenderableTwo {
    float vertices[sizeof(_QuadVertices) / sizeof(float)];
    float *Vertices() override {
        return vertices;
    }
    unsigned int VertexCount() override {
        return sizeof(_QuadVertices) / sizeof(float) / _QuadVerticesSize;
    }
    unsigned int VertexSize() override {
        return _QuadVerticesSize;
    };
};

struct Rectangle *NewRectangle();

struct ModelObject : public ShaderAttributes {
    glm::mat4 model;
    void Attributes(struct VertexBufferAttributes *attrs) {
        attrs->Add4(2, GL_FLOAT, 4, 1); // model
    }
};
struct TexturedObject : ModelObject {
    unsigned int spriteId;
    void Attributes(struct VertexBufferAttributes *attrs) {
        ModelObject::Attributes(attrs);
        attrs->Add(6, GL_INT, 1, 1); // SpriteID
    }
};
} // namespace enginepp::graphics::objects::test
