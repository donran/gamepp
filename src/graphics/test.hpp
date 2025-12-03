#pragma once
#include <GL/glew.h>
#include <string>

extern GLuint createTriangle(float _size);
extern GLuint loadShaderFromStrings(const std::string &_vertex, const std::string &_fragment);
