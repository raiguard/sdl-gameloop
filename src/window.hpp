#pragma once
#include <memory>

class DebugGui;
class Widget;
struct ImGuiIO;
struct ImVec2;
struct SDL_Renderer;
struct SDL_Window;
struct State;
union SDL_Event;

class Window
{
public:
  Window();
  ~Window();
  bool handleEvent(SDL_Event& event);
  void prepare();
  void draw(State& state);
  int getWidth() { return this->width; }
  int getHeight() { return this->height; }
  DebugGui& getDebugGui() { return *this->debugGui; }

  bool useVsync = true;
  bool updateVsync = true;

private:
  void init();

  SDL_Window* window;
  SDL_Renderer* renderer;

  std::unique_ptr<Widget> baseWidget;
  std::unique_ptr<DebugGui> debugGui;

  bool needsResize = false;
  int width = 1920;
  int height = 1080;
};
