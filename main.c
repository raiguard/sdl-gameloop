#include <SDL2/SDL.h>

#include "util.c"
#include "window.c"

int main(int argc, char** argv)
{
  setbuf(stdout, NULL);
  logEnv();

  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  SDL_Init(SDL_INIT_EVERYTHING);

  Window window = windowNew("SDL Test");

  logDisplayInfo(window);

  SDL_Event ev;
  uint quit = 0;
  while (!quit)
  {
    if (SDL_PollEvent(&ev) == 0)
    {
      SDL_Delay(1);
      continue;
    }
    switch (ev.type)
    {
    case SDL_CONTROLLERDEVICEADDED:
      windowNewController(window, ev);
      break;
    case SDL_CONTROLLERDEVICEREMOVED:
      windowFinishController(window, ev.cdevice.which);
      break;
    case SDL_WINDOWEVENT:
      if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
        windowRefreshSurface(window);
      break;
    case SDL_WINDOWEVENT_SHOWN:
      windowRefreshSurface(window);
      break;
    case SDL_CONTROLLERBUTTONDOWN:
      printf("%s\n", SDL_GameControllerGetStringForButton(ev.cbutton.button));
      break;
    case SDL_QUIT:
      quit = 1;
      break;
    }
  }

  windowFinish(window);

  SDL_Quit();

  return 0;
}
