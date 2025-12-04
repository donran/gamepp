#pragma once
#include <GL/glew.h>
#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <vector>

static std::array<GLuint, 6> shaderTypes = {GL_COMPUTE_SHADER,      GL_VERTEX_SHADER,
                                            GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER,
                                            GL_GEOMETRY_SHADER,     GL_FRAGMENT_SHADER};
bool _compile_ok(GLuint objId, bool debugPrint, GLenum _mode);
namespace enginepp::graphics::shaders {

class Shader {
  private:
    GLuint m_id;
    GLuint m_type;

  public:
    Shader(GLuint shaderType) {
        if (std::find(shaderTypes.begin(), shaderTypes.end(), shaderType) == shaderTypes.end()) {
            // Invalid shader type
            return;
        }
        m_type = shaderType;
        m_id = glCreateShader(m_type);
    }

    ~Shader() {
        glDeleteShader(m_id);
    }

    operator GLuint() const {
        return this->m_id;
    }

    bool Compile(const std::string &src) {
        auto srcptr = src.c_str();
        glShaderSource(m_id, 1, &srcptr, NULL);
        glCompileShader(m_id);
        return _compile_ok(m_id, true, GL_COMPILE_STATUS);
    }
};

class Program {
  private:
    GLuint m_id;
    std::vector<Shader *> m_compiledShaders;
    std::map<std::string, GLuint> m_uniformLocations;

  public:
    Program() {
        m_id = glCreateProgram();
    }

    ~Program() {
        glDeleteProgram(m_id);
    }

    bool Add(GLuint type, const std::string &src) {

        Shader *s = new Shader(type);
        if (!s->Compile(src)) {
            return false;
        }
        m_compiledShaders.push_back(s);
        return true;
    }

    bool Link() {
        for (auto &shader : m_compiledShaders) {
            glAttachShader(m_id, *shader);
        }
        glLinkProgram(m_id);
        for (auto &shader : m_compiledShaders) {
            delete shader;
        }
        m_compiledShaders.clear(); // cleanup, class glDeleteShader

        GLint res;
        glGetProgramiv(m_id, GL_LINK_STATUS, &res);
        return res == GL_TRUE; // _compile_ok(m_id, true, GL_LINK_STATUS);
    }

    void Use() {
        glUseProgram(m_id);
    }

    GLint UniformLocation(const std::string name) {
        if (m_uniformLocations.count(name) == 0) {
            m_uniformLocations[name] = glGetUniformLocation(m_id, name.c_str());
        }

        return m_uniformLocations[name];
    }

    void UniformMat4f(const std::string &name, GLfloat *value, GLsizei count = 1,
                      GLboolean transpose = GL_FALSE) {
        glUniformMatrix4fv(UniformLocation(name), count, transpose, value);
    }
};

}; // namespace enginepp::graphics::shaders
