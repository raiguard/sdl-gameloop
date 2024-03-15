#pragma once
#include <memory>
#include <SDL3/SDL.h>

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
  void render(State& state);
  int getWidth() const { return this->width; }
  int getHeight() const { return this->height; }
  std::pair<int, int> getScaledSize() const;
  DebugGui& getDebugGui() const { return *this->debugGui; }

  bool useVsync = true;
  bool updateVsync = true;
  bool occluded = false;

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
