#pragma once

#include "graphics/objects/vao.hpp"
#include "graphics/textures/texture.hpp"
#include <functional>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <vector>

using namespace enginepp::graphics::objects;
using namespace enginepp::graphics::textures;
#include "graphics/objects/newidea2.hpp"
#include "stb/image.hpp"
using namespace enginepp::graphics::objects::test;

template <typename T>
concept RenderableConcept = std::is_base_of_v<RenderableTwo, T>;

class GraphicsLayerFuck {
  private:
    unsigned char m_layerId;
    VAO m_vao;
    std::vector<VBO *> m_buffers;
    GLTexture *m_textures[32] = {0};

  public:
    GraphicsLayerFuck(unsigned char layer_id) : m_layerId(layer_id) {
        m_vao.Bind();
        std::cout << "this is called?" << std::endl;
    }

    void AddTexture(unsigned char texUnit, enginepp::stb::Image *img) {
        m_vao.Bind();
        GLTexture *new_tex = new GLTexture();
        new_tex->Bind();
        glActiveTexture(GL_TEXTURE0 + texUnit);
        new_tex->SetTexture(img);
        m_textures[texUnit] = new_tex;
    }

    template <RenderableConcept T>
    void CreateBuffer(std::function<void(VBO *)> vbo_callback, T *obj = nullptr) {
        VBO *new_buffer = new VBO;
        m_vao.Bind();
        new_buffer->Bind();
        if (obj == nullptr) {
            // Is this the static buffer?
            obj = new T;

            // TODO: but this will leak, so uh, what
            new_buffer->BufferData(1, obj);
        }
        if (std::is_base_of_v<ShaderAttributes, T>) {
            auto shader_attr_obj = dynamic_cast<ShaderAttributes *>(obj);
            shader_attr_obj->Attributes(new_buffer->GetAttributes());
        }
    }

    void Render(unsigned int count) {
        m_vao.Bind();
        for (auto tex : m_textures) {
            if (tex != nullptr) {
                tex->Bind();
            }
        }

        std::cout << "rendering layer " << (int)m_layerId << std::endl;
    }
};

class GraphicsPipeline {
  private:
    GraphicsLayerFuck *m_layers[32] = {0};

  public:
    GraphicsPipeline() {
    }

    void AddLayer(unsigned char layer_id, std::function<void(GraphicsLayerFuck *)> layer_creation_cb) {
        GraphicsLayerFuck **curr_layer = &m_layers[layer_id];
        if (*curr_layer != nullptr) {
            std::cout << "layer " << layer_id << " already eixsts, deleting" << std::endl;
            delete *curr_layer;
        }
        *curr_layer = new GraphicsLayerFuck(layer_id);

        layer_creation_cb(*curr_layer);
    }

    void Render(unsigned char layer_id, unsigned int count) {
        GraphicsLayerFuck *layer = m_layers[layer_id];
        if (layer == nullptr) {
            return;
        }

        for (auto layer : m_layers) {
            if (layer != nullptr) {
                layer->Render(count);
            }
        }
    }
};
struct OurThingy : public ShaderAttributes {
    glm::mat4 model;
    unsigned int spriteId;
    static void Attributes(struct VertexBufferAttributes *attrs) {
        std::cout << "adding ModelObject" << std::endl;
        attrs->Add4(2, GL_FLOAT, 4, 1); // model
        std::cout << "adding TexturedObject" << std::endl;
        attrs->Add(6, GL_INT, 1, 1); // SpriteID
    }

    void SetPosition(glm::vec2 pos) {
        model = ModelMatrix(pos); // * glm::scale(glm::mat4(1.0f), glm::vec3(500.0f, 500.0f, 1.0f));
    }
    glm::mat4 ModelMatrix(glm::vec2 pos) {
        auto modelPos = glm::vec3(pos, 0.0f);
        auto modelTranslate = glm::translate(glm::mat4(1.0f), modelPos);
        return modelTranslate;
    }
};
#include "testing_layer.hpp"
GraphicsLayer<RectangleTwo, OurThingy> *testing();
void update_testing(GraphicsLayer<RectangleTwo, OurThingy> *pipeline, GLTexture *tex);
