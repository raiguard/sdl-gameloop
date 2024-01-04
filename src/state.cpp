#include "state.hpp"
#include <format>
#include <map>
#include <optional>
#include <ostream>
#include <SDL_events.h>
#include <SDL_scancode.h>

void State::handleEvent(SDL_Event& event)
{
  if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
    return;

  SDL_Scancode& sc = event.key.keysym.scancode;
  if (!this->heldKeys.contains(sc))
    return;

  this->heldKeys[sc] = event.type == SDL_KEYDOWN;
}

void State::update()
{
  if (this->heldKeys[SDL_SCANCODE_W])
    this->position.y -= 1;
  else if (this->heldKeys[SDL_SCANCODE_S])
    this->position.y += 1;

  if (this->heldKeys[SDL_SCANCODE_A])
    this->position.x -= 1;
  else if (this->heldKeys[SDL_SCANCODE_D])
    this->position.x += 1;

  this->counter++;
}
