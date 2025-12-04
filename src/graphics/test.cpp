#include "test.hpp"
#include <array>
#include <iostream>
#include <memory>

void printInfoLog(const GLuint &_obj, GLenum _mode = GL_COMPILE_STATUS) {
    GLint infologLength = 0;
    GLint charsWritten = 0;
    std::unique_ptr<char[]> infoLog;

    glGetShaderiv(_obj, GL_INFO_LOG_LENGTH, &infologLength);
    std::cerr << "info log length " << infologLength << "\n";
    if (infologLength > 0) {
        infoLog = std::make_unique<char[]>(infologLength);
        glGetShaderInfoLog(_obj, infologLength, &charsWritten, &infoLog[0]);

        std::cerr << infoLog.get() << '\n';
        glGetShaderiv(_obj, _mode, &infologLength);
        if (infologLength == GL_FALSE) {
            if (_mode == GL_COMPILE_STATUS)
                std::cerr << "Shader compile failed or had warnings \n";
            else
                std::cerr << "linker error\n";
            exit(EXIT_FAILURE);
        }
    }
}
GLuint loadShaderFromStrings(const std::string &_vertex, const std::string &_fragment) {
    // here we create a program
    GLuint shaderID = glCreateProgram();

    // create a Vertex shader object
    GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
    // attatch the shader source we need to convert to GL format
    const char *source = _vertex.c_str();
    glShaderSource(vertexID, 1, &source, NULL);
    // now compile the shader
    glCompileShader(vertexID);
    std::cerr << "compiling vertex shader\n";
    printInfoLog(vertexID);

    // now create a fragment shader
    GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    // attatch the shader source
    source = _fragment.c_str();
    glShaderSource(fragmentID, 1, &source, NULL);
    // compile the shader
    std::cerr << "compiling frag shader shader\n";
    glCompileShader(fragmentID);
    printInfoLog(fragmentID);
    // now attach to the program object
    glAttachShader(shaderID, vertexID);
    glAttachShader(shaderID, fragmentID);

    // link the program
    glLinkProgram(shaderID);
    printInfoLog(shaderID, GL_LINK_STATUS);
    // and enable it for use
    glUseProgram(shaderID);
    // now tidy up the shaders as we don't need them
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    return shaderID;
}
