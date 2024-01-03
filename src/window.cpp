#include "window.hpp"
#include <format>
#include <iostream>
#include <SDL2/SDL.h>

Window::Window(const char* title)
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  this->window = SDL_CreateWindow(title, 0, 0, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
  SDL_FillRect(this->surface, nullptr, SDL_MapRGB(this->surface->format, 0, 0, 0));
  int width = this->surface->w / 3;
  int height = this->surface->h / 3;
  SDL_Rect fillRect = {this->surface->w / 2 - width / 2, this->surface->h / 2 - height / 2, width, height};
  SDL_FillRect(this->surface, &fillRect, SDL_MapRGB(this->surface->format, 255, 0, 0));
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
