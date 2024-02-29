#include "debuggui.hpp"
#include "state.hpp"
#include "window.hpp"
#include "worker.hpp"
#include <chrono>
#include <imgui.h>
#include <SDL2/SDL.h>
#include <mutex>
#include <thread>

enum class ProcessResult
{
  Success,
  Quit,
};

ProcessResult handleEvents(State& state, Window& window)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
      return ProcessResult::Quit;

    if (window.handleEvent(event))
      continue;

    state.handleEvent(event);
  }

  return ProcessResult::Success;
}

std::mutex updateMutex;
std::atomic<bool> didUpdate(false);

void updateLoop(State& state)
{
  std::unique_lock<std::mutex> lock(updateMutex);

  using Clock = std::chrono::high_resolution_clock;
  using namespace std::chrono_literals;

  static Clock::time_point lastFrameTime = Clock::now();
  static Clock::duration accumulator;
  static constexpr std::chrono::nanoseconds timestep(int(1000000000 / 60));

  Clock::time_point currentTime = Clock::now();
  Clock::duration frameTime = currentTime - lastFrameTime;
  lastFrameTime = currentTime;

  accumulator += frameTime;

  while (accumulator >= timestep)
  {
    state.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(2)); // Simulate load
    accumulator -= timestep;
    didUpdate = true;
  }

  // // Cap framerate at max simulation speed
  // std::chrono::duration thisFrameTime = Clock::now() - lastFrameTime;
  // Clock::duration timeToSleep = timestep - accumulator - thisFrameTime;
  // if (timeToSleep > Clock::duration::zero())
  //   std::this_thread::sleep_for(timeToSleep);
}

ProcessResult mainLoopStep(Worker& updateWorker, State& state, Window& window)
{
  {
    ProcessResult result = handleEvents(state, window);
    if (result != ProcessResult::Success)
      return result;
  }

  bool didPrepare = false;
  if (didUpdate)
  {
    window.prepare();
    didPrepare = true;
    didUpdate = false;
  }

  updateWorker.run([&] { updateLoop(state); });

  // Only render if we did a prepare
  if (didPrepare)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(2)); // Simulate load
    window.draw(state);
  }
  else
    // We are in the state that rendering has finished and the updateThread from the previous loop did not do an update, because it was too soon
    // this means there is nothing to do until the next game update, so it usualy safe to sleep
    // depending on how much we sleep here for, this will cause some jitter
    std::this_thread::sleep_for(std::chrono::microseconds(500));

  return ProcessResult::Success;
}

int main(int argc, char** argv)
{
  State state;
  Window window;
  Worker updateWorker;

  ProcessResult result = ProcessResult::Success;
  while (result == ProcessResult::Success)
    result = mainLoopStep(updateWorker, state, window);

  return 0;
}
