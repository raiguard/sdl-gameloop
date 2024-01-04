#pragma once
#include <memory>
#include <string_view>

class Gui;
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
  void handleEvent(SDL_Event& event);
  void update(State& state);
  void draw(State& state);

  Gui& getGui() { return *this->gui; }

private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  Gui* gui;

  bool needsResize = false;
};
