#pragma once
#include <SDL_scancode.h>
#include <imgui.h>
#include <map>

union SDL_Event;

struct State
{
  void handleEvent(SDL_Event& event);
  void update();

  ImVec2 position;

  std::map<SDL_Scancode, bool> heldKeys = {
    {SDL_SCANCODE_W, false},
    {SDL_SCANCODE_A, false},
    {SDL_SCANCODE_S, false},
    {SDL_SCANCODE_D, false},
  };
};
