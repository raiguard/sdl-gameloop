#include "window.hpp"
#include <format>
#include <iostream>
#include <SDL2/SDL.h>

Window::Window(const char* title)
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN);
  this->refreshSurface();
  this->logEnv();
  this->logDisplayInfo();
}

Window::~Window()
{
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Window::refreshSurface()
{
  this->surface = SDL_GetWindowSurface(this->window);
  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0, 0, 0));
  SDL_UpdateWindowSurface(this->window);
}

void Window::logEnv()
{
  printf(
      "Environment: DISPLAY=%s WAYLAND_DISPLAY=%s DESKTOP_SESSION=%s XDG_SESSION_DESKTOP=%s XDG_CURRENT_DESKTOP=%s\n",
      getenv("DISPLAY"), getenv("WAYLAND_DISPLAY"), getenv("DESKTOP_SESSION"), getenv("XDG_SESSION_DESKTOP"),
      getenv("XDG_CURRENT_DESKTOP"));
}

void Window::logDisplayInfo()
{
  int numberOfAdapters = SDL_GetNumVideoDisplays();
  printf("Available displays: %d\n", numberOfAdapters);
  for (int i = 0; i < numberOfAdapters; i++)
  {
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(i, &rect) == 0)
    {
      const char* name = SDL_GetDisplayName(i);
      printf("%s\n", name);
    }
  }
}
