#pragma once

class Window;
struct ImGuiIO;
struct SDL_Renderer;
struct SDL_Window;
struct State;
union SDL_Event;

class Gui
{
public:
  Gui(SDL_Window* window, SDL_Renderer* renderer);
  ~Gui();
  void handleEvent(SDL_Event& event);
  void update(State& state);

  ImGuiIO* io;
};
