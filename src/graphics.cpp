#include "graphics.hpp"
#include <glad/glad.h>
#include <iostream>
#include <ostream>

uint32_t Graphics::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
  uint32_t program = glCreateProgram();
  uint32_t vert = Graphics::compileShader(GL_VERTEX_SHADER, vertexShader);
  uint32_t frag = Graphics::compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return program;
}

uint32_t Graphics::compileShader(uint32_t type, const std::string& source)
{
  uint32_t id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char message[length];
    glGetShaderInfoLog(id, length, &length, message);
    // TODO: Print filename
    std::cout << "Failed to compile shader: " << message << std::endl;
  }

  return id;
}
