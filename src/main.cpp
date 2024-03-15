#include "debuggui.hpp"
#include "state.hpp"
#include "window.hpp"
#include <chrono>
#include <imgui.h>
#include <SDL3/SDL.h>
#include <thread>

typedef bool ShouldQuit;

ShouldQuit handleEvents(State& state, Window& window)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_EVENT_QUIT)
      return true;

    if (window.handleEvent(event))
      continue;

    state.handleEvent(event);
  }

  return false;
}

ShouldQuit mainLoopStep(State& state, Window& window)
{
  using Clock = std::chrono::high_resolution_clock;
  using namespace std::chrono_literals;

  static Clock::time_point lastFrameTime = Clock::now();
  static Clock::duration accumulator;
  static constexpr std::chrono::nanoseconds timestep(int(1000000000 / 1000));

  Clock::time_point currentTime = Clock::now();
  Clock::duration frameTime = currentTime - lastFrameTime;
  lastFrameTime = currentTime;

  accumulator += frameTime;

  if (handleEvents(state, window))
    return true;

  while (accumulator >= timestep)
  {
    state.update();
    accumulator -= timestep;
  }

  window.render(state);

  // Cap framerate at max simulation speed
  std::chrono::duration thisFrameTime = Clock::now() - lastFrameTime;
  Clock::duration timeToSleep = timestep - accumulator - thisFrameTime;
  if (timeToSleep > Clock::duration::zero())
    std::this_thread::sleep_for(timeToSleep);

  return false;
}

int main(int argc, char** argv)
{
  State state;
  Window window;

  ShouldQuit quit = false;
  while (!quit)
    quit = mainLoopStep(state, window);

  return 0;
}
