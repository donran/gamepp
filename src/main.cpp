
#include "graphics/camera/camera.hpp"
#include "graphics/objects/sprite.hpp"
#include "graphics/objects/vbo.hpp"
#include "graphics/shaders/shader.hpp"
#include "graphics/textures/texture.hpp"
#include "graphics/window.hpp"
#include "io/files.hpp"
#include "stb/image.hpp"
#include "testing.hpp"
#include <GLFW/glfw3.h>
#include <boost/filesystem.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

using namespace enginepp::graphics;
using namespace enginepp::graphics::objects;
using namespace enginepp::graphics::camera;
using namespace enginepp::graphics::textures;
using namespace enginepp::graphics::shaders;
using namespace enginepp::io;

namespace fs = boost::filesystem;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;
const char *WINDOW_TITLE = "GamePP";

#define _MAIN_ASSERT(cond, msg)        \
    if (!(cond)) {                     \
        std::cerr << msg << std::endl; \
        exit(EXIT_FAILURE);            \
    }

void keyCallback(GLFWwindow *_window, int _key, int _scancode, int _action, int _mods);
int main(int argc, const char **argv) {
    fs::path base_path(".");
    Window *w = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    w->SetVersion(4, 1);
    w->SetForwardCompat(true);
    w->SetProfile(OpenGLProfile::CORE);

    _MAIN_ASSERT(w->Create(), "failed to create window, exiting");
    w->SetDebug(true);
    w->SetKeyCallback(keyCallback);

    glEnable(GL_CULL_FACE);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    std::vector<Sprite> spriteObjs = {Sprite(glm::vec2(0, 0), 0)};

    auto pipe_test = testing();

    // SpriteBuffer sbo;
    // sbo.Buffer(spriteObjs);

    // Texture
    glActiveTexture(GL_TEXTURE0);
    auto testTex = new textures::GLTexture();
    // TOOD: ew
    auto testTexImage = new enginepp::stb::Image((base_path / fs::path("assets/images/lofiEnvironment.png")).c_str());
    _MAIN_ASSERT(testTexImage->Ok(), "failed to load texture image");
    _MAIN_ASSERT(testTex->SetTexture(testTexImage) == 0, "failed to set texture image");

    // shader
    shaders::Program shaderProgram;
    auto vertexSrc = files::read_file((base_path / "assets/shaders/main.vert.glsl").c_str());
    auto fragmentSrc = files::read_file((base_path / "assets/shaders/main.frag.glsl").c_str());
    _MAIN_ASSERT(shaderProgram.Add(GL_VERTEX_SHADER, vertexSrc), "failed to compile vertex shader");
    _MAIN_ASSERT(shaderProgram.Add(GL_FRAGMENT_SHADER, fragmentSrc), "failed to compile fragment shader");
    _MAIN_ASSERT(shaderProgram.Link(), "failed to link shader program");
    shaderProgram.Use();

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
    glm::mat4 proj = camera.ProjectionMatrix();
    glm::mat4 view = camera.ViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f));

    shaderProgram.UniformMat4f("projection", &proj[0][0]);
    shaderProgram.UniformMat4f("view", &view[0][0]);

    double lastSwitch = 0; // glfwGetTime();
    while (!w->ShouldClose()) {
        auto time = glfwGetTime();
        if (time - lastSwitch > 0.5) {
            lastSwitch = time;
            spriteObjs[0].SetSprite((spriteObjs[0].SpriteId() + 1) % (16 * 16));
            // sbo.Buffer(spriteObjs);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        testTex->Bind();
        // sbo.Draw(spriteObjs.size());

        update_testing(pipe_test, testTex);

        // Use pipe_test
        // pipe_test->Render();
        // pipe_test->Draw({});
        // Currently drawingin update_testing

        w->SwapBuffers();

        w->PollEvents();
    }

    delete w;

    return 1;
}

void keyCallback(GLFWwindow *_window, int _key, int _scancode, int _action, int _mods) {
    if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS) {

        glfwSetWindowShouldClose(_window, GLFW_TRUE);
        // exit(EXIT_SUCCESS);
    }
    if (_key == GLFW_KEY_W && _action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (_key == GLFW_KEY_S && _action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
