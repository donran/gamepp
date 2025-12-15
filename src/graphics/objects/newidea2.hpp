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

struct ShaderAttributes {
    // virtual void Attributes(struct VertexBufferAttributes *attrs) = 0;
    static void Attributes(struct VertexBufferAttributes *attrs) {
    }
};
struct RenderableTwo : ShaderAttributes {
    virtual float *Vertices() = 0;
    virtual unsigned int VertexCount() = 0;
    virtual unsigned int VertexSize() = 0;
};

struct Quad : RenderableTwo {
    static void Attributes(struct VertexBufferAttributes *attrs) {
        std::cout << "adding Quad" << std::endl;
        attrs->Add(0, GL_FLOAT, 2); // Position
        attrs->Add(1, GL_FLOAT, 2); // uv/texcoord
    }
};

struct RectangleTwo : public Quad {
    float vertices[sizeof(_QuadVertices) / sizeof(float)];
    float *Vertices() override {
        return _QuadVertices;
    }
    unsigned int VertexCount() override {
        return sizeof(_QuadVertices) / sizeof(float) / _QuadVerticesSize;
    }
    unsigned int VertexSize() override {
        return _QuadVerticesSize;
    };
};

struct Rectangle : public Quad {
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

struct ModelObject : public Rectangle {
    glm::mat4 model;
    static void Attributes(struct VertexBufferAttributes *attrs) {
        Rectangle::Attributes(attrs);
        std::cout << "adding ModelObject" << std::endl;
        attrs->Add4(2, GL_FLOAT, 4, 1); // model
    }
};
struct TexturedObject : ModelObject {
    unsigned int spriteId;
    static void Attributes(struct VertexBufferAttributes *attrs) {
        ModelObject::Attributes(attrs);
        std::cout << "adding TexturedObject" << std::endl;
        attrs->Add(6, GL_INT, 1, 1); // SpriteID
    }
};
} // namespace enginepp::graphics::objects::test
