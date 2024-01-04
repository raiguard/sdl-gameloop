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
  void draw(State& state);
  int getWidth() { return this->width; }
  int getHeight() { return this->height; }

  Gui& getGui() { return *this->gui; }

private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  std::unique_ptr<Gui> gui;

  bool needsResize = false;
  int width = 1920;
  int height = 1080;
};
