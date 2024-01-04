#include "window.hpp"
#include <cassert>
#include <format>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

Window::Window(const char* title)
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  this->window = SDL_CreateWindow(title, 0, 0, this->getWidth(), this->getHeight(),
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  assert(this->window);

  this->logEnv();
  this->logDisplayInfo();

  this->context = SDL_GL_CreateContext(this->window);
  assert(this->context);
}

Window::~Window()
{
  SDL_GL_DeleteContext(this->context);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Window::draw()
{
  if (this->needsResize)
  {
    this->needsResize = false;
    SDL_GetWindowSize(this->window, &this->dimensions.width, &this->dimensions.height);
  }

  glViewport(0, 0, this->getWidth(), this->getHeight());
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);

  SDL_GL_SwapWindow(this->window);
}

void Window::handleWindowEvent(SDL_WindowEvent& event)
{
  switch (event.event)
  {
  case SDL_WINDOWEVENT_RESIZED:
    this->needsResize = true;
    break;
  }
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
