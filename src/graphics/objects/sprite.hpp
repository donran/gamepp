#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#pragma
#include "instanced.hpp"

namespace enginepp::graphics::objects {

static float _QuadVertices[] = {
    -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom Right

    -0.5f, 0.5f,  0.0f, 1.0f, // Top left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom right
    0.5f,  0.5f,  1.0f, 1.0f  // Top right
};

struct Quad {
    float *StaticVertices() {
        return _QuadVertices;
    }
};

struct InstancedObject2D : Quad {
    glm::mat4 model;
};

class Sprite {
  private:
    struct {
        struct InstancedObject2D obj;
        unsigned int spriteId;
    } m_attributes;
    glm::vec2 m_position;

  public:
    Sprite(glm::vec2 pos, unsigned int spriteId) {
        SetPosition(pos);
        SetSprite(spriteId);
    }
    void SetSprite(unsigned int id) {
        m_attributes.spriteId = id;
    }

    unsigned int SpriteId() {
        return m_attributes.spriteId;
    }

    void SetPosition(glm::vec2 pos) {
        m_position = pos;
        m_attributes.obj.model = ModelMatrix(); // * glm::scale(glm::mat4(1.0f), glm::vec3(500.0f, 500.0f, 1.0f));
    }
    glm::mat4 ModelMatrix() {
        auto modelPos = glm::vec3(m_position, 0.0f);
        auto modelTranslate = glm::translate(glm::mat4(1.0f), modelPos);
        return modelTranslate;
    }
};
class SpriteBuffer {

  private:
    InstancedBuffer m_instancedBuffer;
    bool m_initialized = false;

  public:
    SpriteBuffer();
    ~SpriteBuffer() {
    }
    void Buffer(std::vector<Sprite> &rectinfos);
    void Draw(GLsizei count);
    void Bind() {
        m_instancedBuffer.Bind();
    }

    void Unbind() {
        m_instancedBuffer.Bind();
    }
};

}; // namespace enginepp::graphics::objects
