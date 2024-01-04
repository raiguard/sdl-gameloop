#include "gui.hpp"
#include "state.hpp"
#include "window.hpp"
#include <chrono>
#include <imgui.h>
#include <SDL2/SDL.h>
#include <thread>

bool logic(State& state, Window& window)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
      return true;

    window.handleEvent(event);
    if (window.getGui().io->WantCaptureKeyboard)
      continue;

    state.handleEvent(event);
  }

  state.update();
  window.update(state);

  window.render(state);

  return false;
}

static std::chrono::nanoseconds dt = std::chrono::nanoseconds(1000000000 / 144);

void mainLoop(State& state, Window& window)
{
  using clock = std::chrono::steady_clock;

  clock::time_point frameEnd;
  while (true)
  {
    frameEnd = clock::now();
    frameEnd += dt;

    if (logic(state, window))
      return;

    std::this_thread::sleep_until(frameEnd);
  }
}

int main(int argc, char** argv)
{
  setbuf(stdout, NULL);

  State state;
  Window window;

  mainLoop(state, window);

  return 0;
}
