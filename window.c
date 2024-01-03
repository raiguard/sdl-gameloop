#pragma once
#include <SDL2/SDL.h>

typedef struct Window
{
  SDL_Window* window;
  SDL_Surface* surface;

  int gameControllerID;
  SDL_GameController* gameController;
} Window;

void windowFinishController(Window self, int gameControllerID)
{
  if (self.gameControllerID == -1 || self.gameControllerID != gameControllerID)
    return;

  SDL_GameControllerClose(self.gameController);
  self.gameControllerID = -1;
  self.gameController = NULL;
}

void windowNewController(Window self, SDL_Event ev)
{
  if (self.gameControllerID > -1)
    return;

  self.gameControllerID = ev.cdevice.which;
  self.gameController = SDL_GameControllerOpen(self.gameControllerID);
}

void windowFinish(Window self)
{
  windowFinishController(self, -1);
  SDL_DestroyWindow(self.window);
}

void windowRefreshSurface(Window self)
{
  self.surface = SDL_GetWindowSurface(self.window);
  SDL_FillRect(self.surface, NULL, SDL_MapRGB(self.surface->format, 0, 0, 0));
  SDL_UpdateWindowSurface(self.window);
}

Window windowNew(const char* title)
{
  SDL_Window* window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN);

  Window self;
  self.window = window;
  windowRefreshSurface(self);
  return self;
}
