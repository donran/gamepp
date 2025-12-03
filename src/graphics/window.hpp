#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>

#define WINDOW_ASSERT(assertion, errormsg)                                                                                                                     \
    if (!(assertion)) {                                                                                                                                        \
        std::cerr << errormsg << std::endl;                                                                                                                    \
        exit(1);                                                                                                                                               \
    }

void GLAPIENTRY default_DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                                             const void *userParam) {
    std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}
namespace enginepp::graphics {
enum OpenGLProfile {
    CORE = GLFW_OPENGL_CORE_PROFILE,
    ANY = GLFW_OPENGL_ANY_PROFILE,
    COMPAT = GLFW_OPENGL_COMPAT_PROFILE,
};
struct WindowOptions {
    int width;
    int height;
    std::string title;
    // ex: 4.1
    float gl_version;
    enum OpenGLProfile profile;
    bool forward_compatible;
    GLFWkeyfun keycallback;
};

class Window {
  private:
    GLFWwindow *m_window = NULL;

    int m_width;
    int m_height;
    std::string m_title;

    int m_fb_width = 0;
    int m_fb_height = 0;

  public:
    Window(int width, int height, const char *title) {
        if (!glfwInit()) {
            std::cerr << "window failed to init GLFW" << std::endl;
            exit(1);
        }
        m_width = width;
        m_height = height;
        m_title.assign(title);
        // Defaults
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    ~Window() {
        glfwTerminate();
    }

    bool Create() {
        std::cout << "we got here 5" << std::endl;
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
        if (!m_window) {
            std::cerr << "failed to create window" << std::endl;
            glfwTerminate();
            return EXIT_FAILURE;
        }
        MakeContextCurrent();
#ifndef __APPLE__
        std::cout << "we got here 6" << std::endl;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
            return false;
        }
        std::cerr << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
#endif

        // Initial viewport
        GetFramebufferSize();
        SetViewport();
        return true;
    }

    void SetVersion(int major, int minor) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    }
    void SetForwardCompat(bool set) {
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, set ? GL_TRUE : GL_FALSE);
    };
    void SetProfile(enum OpenGLProfile profile) {
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
    }
    void SetDebugContext(bool set) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, set ? GL_TRUE : GL_FALSE);
    }
    void SetDebug(bool set, GLDEBUGPROC cbfunc = default_DebugMessageCallback) {
        SetDebugContext(set);
        if (set) {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(cbfunc, 0);
        } else {
            glDisable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(0, 0);
        }
    }
    void MakeContextCurrent() {
        WINDOW_ASSERT(m_window != NULL, "window is not created setting context");
        glfwMakeContextCurrent(m_window);
    }
    void SetKeyCallback(GLFWkeyfun cbfunc) {
        WINDOW_ASSERT(m_window != NULL, "window is not created when setting key callback");
        glfwSetKeyCallback(m_window, cbfunc);
    }
    void PollEvents() {
        glfwPollEvents();
    }
    void SwapBuffers() {
        WINDOW_ASSERT(m_window != NULL, "window is not created when swapping buffers");
        glfwSwapBuffers(m_window);
    }
    std::pair<int, int> GetFramebufferSize() {
        WINDOW_ASSERT(m_window != NULL, "window is not created when getting framebuffer size");
        glfwGetFramebufferSize(m_window, &m_fb_width, &m_fb_height);
        return std::pair<int, int>(m_fb_width, m_fb_height);
    }
    bool ShouldClose() {
        WINDOW_ASSERT(m_window != NULL, "window is not created when checking if it should close");
        return glfwWindowShouldClose(m_window);
    }
    void ShouldClose(bool should) {
        glfwSetWindowShouldClose(m_window, should);
    }

    void SetViewport() {
        SetViewport(0, 0);
    }
    void SetViewport(int x, int y) {
        SetViewport(x, y, 0, 0);
    }
    void SetViewport(int x, int y, int width, int height) {
        if (m_fb_width == 0 || m_fb_height == 0) {
            GetFramebufferSize();
        }
        glViewport(x, y, width == 0 ? m_fb_width : width, height == 0 ? m_fb_height : height);
    }
};
} // namespace enginepp::graphics

#undef WINDOW_ASSERT
