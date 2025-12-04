#include "shader.hpp"
#include <iostream>
#include <memory>
using namespace enginepp::graphics::shaders;

bool _compile_ok(GLuint objId, bool debugPrint, GLenum _mode) {
    GLint infologLength = 0;
    GLint charsWritten = 0;
    std::unique_ptr<char[]> infoLog;

    glGetShaderiv(objId, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0) {
        if (debugPrint) {
            std::cerr << "info log length " << infologLength << "\n";
        }
        infoLog = std::make_unique<char[]>(infologLength);
        glGetShaderInfoLog(objId, infologLength, &charsWritten, &infoLog[0]);

        std::cerr << infoLog.get() << '\n';
        glGetShaderiv(objId, _mode, &infologLength);
        if (infologLength == GL_FALSE) {
            if (_mode == GL_COMPILE_STATUS) {
                std::cerr << "Shader compile failed or had warnings \n";
                return false;
            } else {
                std::cerr << "linker error\n";
                // exit(EXIT_FAILURE);
                return false;
            }
        }
    }

    return true;
}
