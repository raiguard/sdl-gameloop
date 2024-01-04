#pragma once
#include <imgui.h>
#include <map>
#include <SDL_scancode.h>

union SDL_Event;

struct State
{
  State();
  void handleEvent(SDL_Event& event);
  void update();

  ImVec2 position;
  bool showDemoWindow = false;

  std::map<SDL_Scancode, bool> heldKeys;
};
