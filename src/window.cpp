#include "window.hpp"
#include "gui.hpp"
#include "state.hpp"
#include <cassert>
#include <format>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <iostream>
#include <SDL2/SDL.h>

Window::Window()
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  this->window = SDL_CreateWindow("SDL Demo", 0, 0, this->getWidth(), this->getHeight(),
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  assert(this->window);

  this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  assert(this->renderer);
  this->gui.reset(new Gui(this->window, this->renderer));
}

Window::~Window()
{
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Window::draw(State& state)
{
  if (this->needsResize)
  {
    this->needsResize = false;
    SDL_GetWindowSize(this->window, &this->width, &this->height);
  }

  // GUI

  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  this->getGui().draw(state, *this);

  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO();
  SDL_RenderSetScale(this->renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  if (this->updateVsync)
  {
    this->updateVsync = false;
    SDL_RenderSetVSync(this->renderer, this->useVsync);
  }

  // Playfield

  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderClear(this->renderer);

  SDL_Rect rect = {int(this->getWidth() / 2.0f - 400 / 2.0f - state.position.x),
                   int(this->getHeight() / 2.0f - 400 / 2.0f - state.position.y), 400, 400};
  SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(this->renderer, &rect);

  // Draw GUI on top of playfield

  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(this->renderer);
}

bool Window::handleEvent(SDL_Event& event)
{
  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    this->needsResize = true;
  return this->getGui().handleEvent(event);
}
