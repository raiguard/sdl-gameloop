#pragma once
#include <SDL_scancode.h>
#include <imgui.h>
#include <map>

union SDL_Event;

struct Dimensions
{
  int width, height;
};

struct State
{
  void handleEvent(SDL_Event& event);
  void update();

  Dimensions screenSize = {1920, 1080};
  ImVec2 position;
  bool showDemoWindow;

  std::map<SDL_Scancode, bool> heldKeys = {
    {SDL_SCANCODE_W, false},
    {SDL_SCANCODE_A, false},
    {SDL_SCANCODE_S, false},
    {SDL_SCANCODE_D, false},
  };
};
