#include <SDL2/SDL.h>
#include <stdio.h>

#include "window.c"

void logEnv()
{
  printf(
      "Environment: DISPLAY=%s WAYLAND_DISPLAY=%s DESKTOP_SESSION=%s XDG_SESSION_DESKTOP=%s XDG_CURRENT_DESKTOP=%s\n",
      getenv("DISPLAY"), getenv("WAYLAND_DISPLAY"), getenv("DESKTOP_SESSION"), getenv("XDG_SESSION_DESKTOP"),
      getenv("XDG_CURRENT_DESKTOP"));
}

void logDisplayInfo(Window window)
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

  for (int button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
    printf("%d: %s\n", button, SDL_GameControllerGetStringForButton(button));

  for (int axis = 0; axis < SDL_CONTROLLER_AXIS_MAX; axis++)
    printf("%d: %s\n", axis, SDL_GameControllerGetStringForAxis(axis));
}

