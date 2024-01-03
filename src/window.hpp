#pragma once
#include <SDL2/SDL_video.h>
#include <memory>
#include <string_view>

class Window
{
public:
  Window(const char* title);
  ~Window();
  void refreshSurface();
private:
  void logEnv();
  void logDisplayInfo();

  SDL_Window* window;
  SDL_Surface* surface;

  // int gameControllerID;
  // SDL_GameController* gameController;
};
