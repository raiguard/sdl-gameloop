#include "clipboardtest.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <SDL3/SDL_clipboard.h>
#include <SDL3/SDL_error.h>

namespace ClipboardTest
{
  void copy()
  {
    std::ifstream myfile("huge-blueprint.txt");
    std::string content;
    myfile >> content;
    if (SDL_SetClipboardText(content.c_str()) < 0)
      std::cout << std::format("Failed to copy clipboard text: {}", SDL_GetError()) << std::endl;
    else
      std::cout << "Successfully copied to clipboard" << std::endl;
  }

  void paste()
  {
    char* value = SDL_GetClipboardText();
    std::cout << "Clipboard contents: " << value << std::endl;
    SDL_free(value);
  }
}
