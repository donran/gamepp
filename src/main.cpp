
#include "graphics/camera/camera.hpp"
#include "graphics/objects/sprite.hpp"
#include "graphics/objects/vbo.hpp"
#include "graphics/shaders/shader.hpp"
#include "graphics/shaders/shaders.hpp"
#include "graphics/textures/texture.hpp"
#include "graphics/window.hpp"
#include "stb/image.hpp"
#include <GLFW/glfw3.h>
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

    SpriteBuffer sbo;
    // sbo.Buffer(rectinfos);
    sbo.Buffer(spriteObjs);

    // Texture
    glActiveTexture(GL_TEXTURE0);
    auto testTex = new textures::GLTexture();
    auto testTexImage = new enginepp::stb::Image("../assets/lofiEnvironment.png");
    _MAIN_ASSERT(testTexImage->Ok(), "failed to load texture image");
    _MAIN_ASSERT(testTex->SetTexture(testTexImage) == 0, "failed to load texture image");

    // shader
    shaders::Program shaderProgram;
    _MAIN_ASSERT(shaderProgram.Add(GL_VERTEX_SHADER, vertex2), "failed to compile vertex shader");
    _MAIN_ASSERT(shaderProgram.Add(GL_FRAGMENT_SHADER, fragment2), "failed to compile fragment shader");
    _MAIN_ASSERT(shaderProgram.Link(), "failed to link shader program");
    shaderProgram.Use();

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    double lastSwitch = glfwGetTime();
    while (!w->ShouldClose()) {
        auto time = glfwGetTime();
        if (time - lastSwitch > 0.5) {
            lastSwitch = time;
            // rectinfos[0].spriteId = (rectinfos[0].spriteId + 1) % (16 * 16);
            // sbo.Buffer(rectinfos);
            spriteObjs[0].SetSprite((spriteObjs[0].SpriteId() + 1) % (16 * 16));
            sbo.Buffer(spriteObjs);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glm::mat4 proj = camera.ProjectionMatrix();
        glm::mat4 view = camera.ViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f));
        // view = glm::mat4(1.0f);

        shaderProgram.UniformMat4f("projection", &proj[0][0]);
        shaderProgram.UniformMat4f("view", &view[0][0]);
        // shaderProgram.UniformMat4f("model", &model[0][0]);

        glActiveTexture(GL_TEXTURE0);
        testTex->Bind();
        // sbo.Draw(rectinfos.size());
        sbo.Draw(spriteObjs.size());

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
