#pragma once
#include <imgui.h>
#include <map>
#include <SDL_scancode.h>

union SDL_Event;

struct State
{
  void handleEvent(SDL_Event& event);
  void update();

  ImVec2 position;
  bool showDemoWindow = false;

  std::map<SDL_Scancode, bool> heldKeys = {
      {SDL_SCANCODE_W, false},
      {SDL_SCANCODE_A, false},
      {SDL_SCANCODE_S, false},
      {SDL_SCANCODE_D, false},
  };
};
