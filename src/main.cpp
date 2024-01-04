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
  ImVec2 position;
  bool movingUp = false;
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

      if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_W && !movingUp)
        movingUp = true;
      else if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_W && movingUp)
        movingUp = false;
    }

    if (movingUp)
      position.y -= 5;

    window.draw(position);
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
