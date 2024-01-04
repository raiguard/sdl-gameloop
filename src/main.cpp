#include "state.hpp"
#include "window.hpp"
#include <chrono>
#include <imgui.h>
#include <SDL2/SDL.h>
#include <thread>

void run(Window& window)
{
  using clock = std::chrono::steady_clock;

  SDL_Event event;
  clock::time_point frameEnd;
  clock::duration lastFrameTime;
  State state;
  while (true)
  {
    frameEnd = clock::now();
    std::chrono::nanoseconds dt = std::chrono::nanoseconds(1000000000 / 144);
    frameEnd += dt;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        return;
      ImGuiIO& io = window.handleEvent(event);
      if (io.WantCaptureKeyboard)
        continue;

      state.handleEvent(event);
    }

    state.update();

    window.draw(state);
    std::this_thread::sleep_until(frameEnd);
  }
}

int main(int argc, char** argv)
{
  setbuf(stdout, NULL);

  Window window = Window("SDL Test");

  run(window);

  return 0;
}
