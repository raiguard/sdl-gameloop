#pragma once
#include <SDL3/SDL_video.h>

class Window;
struct ImGuiIO;
struct SDL_Window;
struct State;
union SDL_Event;

class DebugGui
{
public:
  DebugGui(SDL_Window* window, SDL_GLContext context);
  ~DebugGui();
  bool handleEvent(SDL_Event& event);
  void draw(State& state, Window& window);
};
