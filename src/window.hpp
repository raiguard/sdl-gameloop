#pragma once
#include <imgui.h>
#include <memory>
#include <SDL2/SDL_video.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <string_view>

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
  void draw(ImVec2& position);
  ImGuiIO& handleEvent(SDL_Event& event);
  int getWidth() const { return this->dimensions.width; }
  int getHeight() const { return this->dimensions.height; }

private:
  void logEnv();
  void logDisplayInfo();

  SDL_Window* window;
  SDL_Renderer* renderer;

  Dimensions dimensions = {1920, 1080};

  bool needsResize = false;
  bool showDemoWindow = false;
};
