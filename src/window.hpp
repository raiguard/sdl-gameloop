#pragma once
#include <SDL2/SDL_video.h>
#include <memory>
#include <string_view>

struct SDL_WindowEvent;

struct Dimensions
{
  int width;
  int height;
};

class Window
{
public:
  Window(const char* title);
  ~Window();
  void draw();
  void handleWindowEvent(SDL_WindowEvent& event);
  int getWidth() const { return this->dimensions.width; }
  int getHeight() const { return this->dimensions.height; }
private:
  void logEnv();
  void logDisplayInfo();

  SDL_Window* window;
  SDL_GLContext context;

  Dimensions dimensions = {1920, 1080};

  bool needsResize = false;
};
