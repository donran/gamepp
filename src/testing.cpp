#include "testing.hpp"
#include "graphics/objects/newidea2.hpp"
#include "graphics/shaders/shader.hpp"
#include "graphics/textures/texture.hpp"
#include "io/assets.hpp"
#include <boost/filesystem.hpp>
using namespace enginepp::graphics::objects;
using namespace enginepp::graphics;
using namespace enginepp::graphics::objects::test;
using namespace enginepp::io;

#include "testing_layer.hpp"

class SpritePipeline : GraphicsPipeline {
  private:
    GraphicsLayer<RectangleTwo, OurThingy> m_layerOurThingy;
    GLTexture m_texture;
    shaders::Program shaderProgram;

    void loadTexture() {
        auto testTexImage = new enginepp::stb::Image(assets::images::Path("lofiEnvironment.png").c_str());
        if (!testTexImage->Ok()) {
            std::cerr << "failed to load texture image" << std::endl;
        }
        if (m_texture.SetTexture(testTexImage) != 0) {
            std::cerr << "failed to set texture image" << std::endl;
        }
    }

    void loadShaderProgram() {
        if (!shaderProgram.Add(GL_VERTEX_SHADER, assets::shaders::ReadFile("main.vert.glsl"))) {
            std::cerr << "failed to compile vertex shader" << std::endl;
        }
        if (!shaderProgram.Add(GL_FRAGMENT_SHADER, assets::shaders::ReadFile("main.frag.glsl"))) {
            std::cerr << "failed to compile fragment shader" << std::endl;
        }
        if (!shaderProgram.Link()) {
            std::cerr << "failed to link shader program" << std::endl;
        }
        shaderProgram.Use();
    }

  public:
    SpritePipeline() {
        loadTexture();
        loadShaderProgram();
    }

    void Draw() override {
    }
    void Draw(const std::span<OurThingy> &vec) {
        glActiveTexture(GL_TEXTURE0);
        m_texture.Bind();
        m_layerOurThingy.Draw(vec, &m_texture);
    }
    void Update() override {
    }
};
auto vec = new std::vector<OurThingy>;

GraphicsLayer<RectangleTwo, OurThingy> *testing() {
    std::cout << "wut" << std::endl;
    // VertexBufferTwo<Rectangle> *rectBuffer = new VertexBufferTwo<Rectangle>();
    auto *layer = new GraphicsLayer<RectangleTwo, OurThingy>();

    OurThingy testour;
    testour.SetPosition(glm::vec2(0.0f, 0.0f));
    testour.spriteId = 3;
    OurThingy testour2;
    testour2.SetPosition(glm::vec2(1.0f, 1.0f));
    testour2.spriteId = 2;
    vec->push_back(testour);
    vec->push_back(testour2);

    std::cout << "wut2" << std::endl;
    return layer;
    // SpritePipeline *res = new SpritePipeline;

    // fs::path base_path(".");
    // auto testTexImage = new enginepp::stb::Image((base_path /
    // fs::path("assets/images/lofiEnvironment.png")).c_str()); if (!testTexImage->Ok()) {
    //     std::cerr << "failed to load texture image" << std::endl;
    //     exit(1);
    // }
    // res->AddLayer(0, [testTexImage](GraphicsLayerFuck *layer) {
    //     layer->CreateBuffer<TexturedObject>([](VBO *buffer) {
    //     });
    //     layer->AddTexture(1, testTexImage);
    // });

    // return res;
}
void update_testing(GraphicsLayer<RectangleTwo, OurThingy> *_pipeline, GLTexture *tex) {
    _pipeline->Draw(*vec, tex);
}
