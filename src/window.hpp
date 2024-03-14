#pragma once
#include <memory>
#include <SDL2/SDL.h>

class DebugGui;
struct ImGuiIO;
struct ImVec2;
struct SDL_Window;
struct State;
union SDL_Event;

class Window
{
public:
  Window();
  ~Window();
  bool handleEvent(SDL_Event& event);
  void draw(State& state);
  int getWidth() { return this->width; }
  int getHeight() { return this->height; }
  DebugGui& getDebugGui() { return *this->debugGui; }

  bool useVsync = true;
  bool updateVsync = true;

private:
  void init();

  SDL_Window* window;
  SDL_GLContext context;

  std::unique_ptr<DebugGui> debugGui;

  bool needsResize = false;
  int width = 1920;
  int height = 1080;

  uint32_t buffer;

  uint32_t triangleShader;

  float positions[12] = {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f, 0.5f,

    0.5f, 0.5f,
    -0.5f, 0.5f,
    -0.5f, -0.5f,
  };
};
