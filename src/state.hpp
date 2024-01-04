#pragma once
#include <SDL_scancode.h>
#include <imgui.h>
#include <map>

union SDL_Event;

struct State
{
  State();
  void handleEvent(SDL_Event& event);
  void update();

  ImVec2 position;
  bool showDemoWindow;

  std::map<SDL_Scancode, bool> heldKeys;
};
