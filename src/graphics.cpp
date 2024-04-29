#include "graphics.hpp"
#include <format>
#include <glad/glad.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <ostream>

uint32_t Graphics::createShader(const std::string& name)
{
  uint32_t program = glCreateProgram();
  uint32_t vert = Graphics::compileShader(GL_VERTEX_SHADER, std::filesystem::current_path() / "shaders" / (name + ".vert"));
  uint32_t frag = Graphics::compileShader(GL_FRAGMENT_SHADER, std::filesystem::current_path() / "shaders" / (name + ".frag"));

  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return program;
}

uint32_t Graphics::compileShader(uint32_t type, const std::string& filename)
{
  std::stringstream buffer;
  buffer << std::ifstream(filename).rdbuf();
  std::string source = buffer.str();

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
    std::string message;
    message.resize(length);
    glGetShaderInfoLog(id, length, &length, message.data());
    std::cout << std::format("Failed to compile shader {}:\n\t", filename) << message << std::endl;
    exit(-1);
  }

  return id;
}
