#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace enginepp::graphics::camera {
class Camera {
  private:
    float m_width;
    float m_height;

    glm::mat4 m_projection;

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);

    const glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

  public:
    Camera(int window_width, int window_height) {
        m_width = (float)window_width;
        m_height = (float)window_height;
        m_projection = glm::ortho(0.0f, float(m_width), 0.0f, float(m_height), -0.1f, 100.0f);
    }

    glm::mat4 ViewMatrix(glm::vec3 target) {
        // auto dir = glm::normalize(m_position - m_target);
        auto dir = glm::normalize(m_position - target);
        glm::vec3 cameraRight = glm::normalize(glm::cross(m_up, dir));
        glm::vec3 cameraUp = glm::cross(dir, cameraRight);

        return glm::lookAt(m_position, m_position + m_cameraFront, cameraUp);
    }

    glm::mat4 ProjectionMatrix() {
        return m_projection;
    }
};
}; // namespace enginepp::graphics::camera
