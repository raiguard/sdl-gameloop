#pragma once
#include <cstdint>
#include <string>

class Graphics
{
public:
  static uint32_t createShader(const std::string& name);
private:
  static uint32_t compileShader(uint32_t typeconst, const std::string& source);
};
