#include "gui.hpp"
#include "state.hpp"
#include "window.hpp"
#include <chrono>
#include <imgui.h>
#include <SDL2/SDL.h>

bool input(State& state, Window& window)
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

  return false;
}

void update(State& state, Window& window)
{
  state.update();
  window.update(state);
}

void render(State& state, Window& window)
{
  window.draw(state);
}

void mainLoop(State& state, Window& window)
{
  using Clock = std::chrono::steady_clock;
  using namespace std::chrono_literals;

  static Clock::time_point lastFrameTime = Clock::now();
  static Clock::duration accumulator;
  static constexpr std::chrono::nanoseconds timestep(1ms); // Update simulation 1000 times per second.

  bool quit = false;
  while (!quit)
  {
    Clock::time_point currentTime = Clock::now();
    Clock::duration frameTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    accumulator += frameTime;

    quit = input(state, window);

    while (accumulator >= timestep)
    {
      update(state, window);
      accumulator -= timestep;
    }

    render(state, window);

    // Cap framerate at max simulation speed
    Clock::duration timeToSleep = timestep - accumulator - (Clock::now() - lastFrameTime);
    if (timeToSleep > Clock::duration::zero())
      // sleep_for uses chrono::system_clock::now(), which is not steady, so just use regular sleep
      usleep(std::chrono::duration_cast<std::chrono::microseconds>(timeToSleep).count());
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
