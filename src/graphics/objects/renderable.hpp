#include <vector>
namespace enginepp::graphics::objects {
class Renderable {
    // virtual std::vector<float> Vertices() = 0;
    virtual std::vector<float> InstanceData() = 0;
};
}; // namespace enginepp::graphics::objects
