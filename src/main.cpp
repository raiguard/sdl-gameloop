#include "debuggui.hpp"
#include "state.hpp"
#include "window.hpp"
#include <chrono>
#include <imgui.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <thread>

bool handleEvents(State& state, Window& window)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
      return true;

    if (window.handleEvent(event))
      continue;

    state.handleEvent(event);
  }

  return false;
}

void mainLoop(State& state, Window& window)
{
  using Clock = std::chrono::high_resolution_clock;
  using namespace std::chrono_literals;

  static Clock::time_point lastFrameTime = Clock::now();
  static Clock::duration accumulator;
  static constexpr std::chrono::nanoseconds timestep(int(1000000000 / 1000));

  bool quit = false;
  while (!quit)
  {
    Clock::time_point currentTime = Clock::now();
    Clock::duration frameTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    accumulator += frameTime;

    quit = handleEvents(state, window);

    while (accumulator >= timestep)
    {
      state.update();
      accumulator -= timestep;
    }

    window.draw(state);

    // Cap framerate at max simulation speed
    std::chrono::duration thisFrameTime = Clock::now() - lastFrameTime;
    Clock::duration timeToSleep = timestep - accumulator - thisFrameTime;
    if (timeToSleep > Clock::duration::zero())
      std::this_thread::sleep_for(timeToSleep);

    // Wayland + Vsync will start lagging when on another workspace
    static constexpr std::chrono::milliseconds maxFrameTime(50);
    if (thisFrameTime > maxFrameTime)
      std::cout << std::format("Taking too long! Are we on another workspace? timeToRender={}",
                               std::chrono::duration_cast<std::chrono::milliseconds>(thisFrameTime))
                << std::endl;
  }
}

int main(int argc, char** argv)
{
  State state;
  Window window;

  mainLoop(state, window);

  return 0;
}
