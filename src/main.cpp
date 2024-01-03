#include "window.hpp"
#include <SDL2/SDL.h>

int main(int argc, char** argv)
{
  setbuf(stdout, NULL);

  Window window = Window("SDL Test");

  SDL_Event ev;
  bool quit = 0;
  while (!quit)
  {
    if (SDL_PollEvent(&ev) == 0)
    {
      SDL_Delay(1);
      continue;
    }
    switch (ev.type)
    {
    case SDL_WINDOWEVENT:
      if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
        window.refreshSurface();
      break;
    case SDL_WINDOWEVENT_SHOWN:
      window.refreshSurface();
      break;
    case SDL_QUIT:
      quit = true;
      break;
    }
  }

  return 0;
}
