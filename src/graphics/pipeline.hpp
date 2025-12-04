#pragma once

#include "objects/sprite.hpp"
#include "shaders/shader.hpp"
#include "shaders/shaders.hpp"
#include "textures/texture.hpp"
#include <vector>

enum GraphicsPipelineError {
    GPIPELINE_OK = 0,
    GPIPELINE_INVALID_IMG,
    GPIPELINE_TEXTURE_FAILURE,
    GPIPELINE_SHADER_COMPILATION_FAILURE,
    GPIPELINE_SHADER_LINK_FAILURE,
};

namespace enginepp::graphics {
class GraphicsPipeline {
  private:
    shaders::Program m_shaderProgram;
    std::map<unsigned int, textures::GLTexture *> m_textures;

  protected:
    GraphicsPipeline() {
    }

    ~GraphicsPipeline() {
    }

    shaders::Program Shader() {
        return m_shaderProgram;
    }

    GraphicsPipelineError AddTextureUnit(GLuint unit, stb::Image &img) {
        glActiveTexture(GL_TEXTURE0 + unit);
        if (!img.Ok()) {
            return GPIPELINE_INVALID_IMG;
        }
        auto newTexture = new textures::GLTexture();
        if (newTexture->SetTexture(&img) != 0) {
            delete newTexture;
            return GPIPELINE_TEXTURE_FAILURE;
        }
        m_textures[unit] = newTexture;

        return GPIPELINE_OK;
    }

    GraphicsPipelineError AddShader(GLenum type, const std::string &src) {
        if (!m_shaderProgram.Add(type, src)) {
            return GPIPELINE_SHADER_COMPILATION_FAILURE;
        }

        return GPIPELINE_OK;
    }

    GraphicsPipelineError Build() {
        if (!m_shaderProgram.Link()) {
            return GPIPELINE_SHADER_LINK_FAILURE;
        }
        return GPIPELINE_OK;
    }

    GraphicsPipelineError InitializeDraw() {
        m_shaderProgram.Use();
        return GPIPELINE_OK;
    }

    virtual GraphicsPipelineError Draw(GLsizei count) = 0;
};

class SpritePipeline : GraphicsPipeline {
  private:
    objects::SpriteBuffer m_spriteBuffer;

  public:
    SpritePipeline() : GraphicsPipeline() {
        enginepp::stb::Image testTexImage("../assets/lofiEnvironment.png");
        if (AddTextureUnit(0, testTexImage) != GPIPELINE_OK) {
            std::cerr << " failed to create texture" << std::endl;
            return;
        }

        if (AddShader(GL_VERTEX_SHADER, vertex2) != GPIPELINE_OK) {
            std::cerr << "failed to created vertex shader" << std::endl;
            return;
        }

        if (AddShader(GL_FRAGMENT_SHADER, fragment2) != GPIPELINE_OK) {
            std::cerr << "failed to created fragment shader" << std::endl;
            return;
        }

        if (Build() != GPIPELINE_OK) {
            std::cerr << "failed to build sprite pipeline" << std::endl;
            return;
        }
    }

    GraphicsPipelineError Buffer(std::vector<objects::Sprite> &rectinfos) {
        m_spriteBuffer.Buffer(rectinfos);
        return GPIPELINE_OK;
    }

    GraphicsPipelineError Draw(GLsizei count) {
        m_spriteBuffer.Draw(count);
        return GPIPELINE_OK;
    }
};
}; // namespace enginepp::graphics
