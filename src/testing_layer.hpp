#pragma once

// So we commit to just doing one static buffer and one instanced buffer?
// Feels easier to actually get going with other stuff
#include "graphics/objects/newidea2.hpp"
#include "graphics/objects/vao.hpp"
#include "graphics/objects/vbo.hpp"
#include "graphics/textures/texture.hpp"
#include <span>

using namespace enginepp::graphics::objects::test;
using namespace enginepp::graphics::objects;

template <typename T>
concept StaticVertexConcept = std::is_base_of_v<RenderableTwo, T>;
template <typename T>
concept InstancedVertexConcept = std::is_base_of_v<ShaderAttributes, T>;

template <InstancedVertexConcept TBase>
class VertexBufferTwo : public VBO {
  private:
    bool m_initialized = false;

  public:
    VertexBufferTwo() : VBO() {
        Attributes([](VertexBufferAttributes *attrs) {
            TBase::Attributes(attrs);
        });
    }
    void Data(const std::span<TBase> &elements) {
        // std::cout << "wut????" << std::endl;
        if (!m_initialized) {
            BufferData(elements.size(), &elements[0], GL_DYNAMIC_DRAW);
            m_initialized = true;
        } else {
            UpdateData(elements.size(), &elements[0]);
        }
    }
};

template <StaticVertexConcept TS, InstancedVertexConcept TI>
class GraphicsLayer {
  private:
    VertexBufferTwo<TS> *m_bufferStatic;
    VertexBufferTwo<TI> *m_bufferInstanced;
    VAO m_ao;

    RenderableTwo *m_ts_obj = new TS;

  public:
    GraphicsLayer() : m_ao() {
        m_ao.Bind();
        std::cout << "so we can bind first " << std::endl;
        m_bufferStatic = new VertexBufferTwo<TS>();
        m_bufferStatic->BufferData(m_ts_obj->VertexCount(), m_ts_obj->Vertices(), GL_STATIC_DRAW);
        m_bufferInstanced = new VertexBufferTwo<TI>();
        m_ao.EnableVertexBufferObjects<VertexBufferTwo<TS>>({*m_bufferStatic});
        m_ao.EnableVertexBufferObjects<VertexBufferTwo<TI>>({*m_bufferInstanced});
    }
    void Draw(const std::span<TI> &elements, enginepp::graphics::textures::GLTexture *tex) {
        std::cout << "draw len: " << elements.size() << std::endl;
        m_ao.Bind();
        tex->Bind();
        glActiveTexture(GL_TEXTURE0);
        m_bufferInstanced->Data(elements);
        glDrawArraysInstanced(GL_TRIANGLES, 0, m_ts_obj->VertexCount(), elements.size());
    }
};
