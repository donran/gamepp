#include "testing.hpp"
#include "graphics/objects/newidea2.hpp"
#include "graphics/objects/vbo.hpp"
using namespace enginepp::graphics::objects;
using namespace enginepp::graphics::objects::test;

#include <type_traits>

template <typename T>
concept RenderableConcept = std::is_base_of_v<T, RenderableTwo>;

class GraphicsPipeline {
  private:
  public:
    GraphicsPipeline() {
    }

    template <RenderableConcept T>
    void AddVertexObject() {
        T tmpObj;
        RenderableTwo *tmp = &tmpObj;
    }
};
void testing() {
    VBO vbo1;
    vbo1.Attributes([](struct VertexBufferAttributes *attrs) {
    });
}
