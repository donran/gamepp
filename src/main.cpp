
#include "graphics/camera/camera.hpp"
#include "graphics/objects/rect.hpp"
#include "graphics/objects/vao.hpp"
#include "graphics/objects/vbo.hpp"
#include "graphics/shaders.hpp"
#include "graphics/test.hpp"
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

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 720;
const char *WINDOW_TITLE = "GamePP";

void keyCallback(GLFWwindow *_window, int _key, int _scancode, int _action, int _mods);
int main(int argc, const char **argv) {
    Window *w = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    w->SetVersion(4, 1);
    w->SetForwardCompat(true);
    w->SetProfile(OpenGLProfile::CORE);

    if (!w->Create()) {
        std::cerr << "failed to create window, exiting" << std::endl;
        return 1;
    }
    w->SetDebug(true);

    w->SetKeyCallback(keyCallback);

    glEnable(GL_CULL_FACE);

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    float quadVertices[] = {
        -0.5f, 0.5f,  0.0f, 1.0f, // Top left
        -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
        0.5f,  -0.5f, 1.0f, 0.0f, // Bottom Right

        -0.5f, 0.5f,  0.0f, 1.0f, // Top left
        0.5f,  -0.5f, 1.0f, 0.0f, // Bottom right
        0.5f,  0.5f,  1.0f, 1.0f  // Top right
    };
    std::vector<Rectangle> rectinfos = {Rectangle{
        .position = glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
        .color = glm::vec4(0.25f, 0.50f, 0.75f, 1.0f),
        .spriteId = 0,
    }};

    VBO quad;

    quad.Attributes([](struct VertexBufferAttributes *attrs) {
        attrs->Add(0, GL_FLOAT, 2); // Position
        attrs->Add(1, GL_FLOAT, 2); // Color
    });
    quad.BufferData(sizeof(quadVertices) / 4, quadVertices);

    VBO instancedVertex;
    instancedVertex.Attributes([](struct VertexBufferAttributes *attrs) {
        attrs->Add(2, GL_FLOAT, 2, 1); // Position
        attrs->Add(3, GL_FLOAT, 4, 1); // Color
        attrs->Add(4, GL_INT, 1, 1);   // SpriteID
    });
    instancedVertex.BufferData(rectinfos.size(), &rectinfos[0], GL_DYNAMIC_DRAW);
    instancedVertex.Unbind();

    VAO vertexArray;
    vertexArray.Bind();
    vertexArray.EnableVertexBufferObjects({quad, instancedVertex});
    vertexArray.Unbind();

    // Texture
    glActiveTexture(GL_TEXTURE0);
    auto testTex = new textures::GLTexture();
    auto testTexImage = new enginepp::stb::Image("../assets/lofiEnvironment.png");
    if (!testTexImage->Ok()) {
        std::cout << "failed to load texture image\n" << std::endl;
        return EXIT_FAILURE;
    }

    if (testTex->SetTexture(testTexImage) != 0) {
        std::cout << "failed to setting texture image\n" << std::endl;
        return EXIT_FAILURE;
    }

    auto shaderID = loadShaderFromStrings(vertex2, fragment2);

    int vertexProjectionLocation = glGetUniformLocation(shaderID, "projection");
    int vertexViewLocation = glGetUniformLocation(shaderID, "view");
    int vertexModelLocation = glGetUniformLocation(shaderID, "model");

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    // glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 1.0f));
    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(500.0f, 500.0f, 1.0f));
    auto modelPos = glm::vec3(rectinfos[0].position, 0.0f);
    auto modelTranslate = glm::translate(glm::mat4(1.0f), modelPos);
    glm::mat4 model = modelTranslate * scale; // glm::translate(scale, );
    double lastSwitch = glfwGetTime();
    while (!w->ShouldClose()) {
        auto time = glfwGetTime();
        if (time - lastSwitch > 0.5) {
            lastSwitch = time;
            rectinfos[0].spriteId = (rectinfos[0].spriteId + 1) % (16 * 16);
            instancedVertex.UpdateData(rectinfos.size(), &rectinfos[0]);
            // std::cout << "switching to " << rectinfos[0].spriteId << std::endl;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glm::mat4 proj = camera.ProjectionMatrix();
        glm::mat4 view = camera.ViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f));
        // view = glm::mat4(1.0f);

        glUniformMatrix4fv(vertexProjectionLocation, 1, GL_FALSE, &proj[0][0]);
        glUniformMatrix4fv(vertexViewLocation, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(vertexModelLocation, 1, GL_FALSE, &model[0][0]);

        glActiveTexture(GL_TEXTURE0);
        testTex->Bind();
        vertexArray.Bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, rectinfos.size());
        vertexArray.Unbind();
        // testTex->Unbind();

        w->SwapBuffers();

        w->PollEvents();
    }

    glDeleteProgram(shaderID);
    // glDeleteVertexArrays(1, &vaoID);

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
