#include "debuggui.hpp"
#include "widget.hpp"
#include "window.hpp"
#include "state.hpp"
#include <cassert>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL.h>
#include <thread>

Window::Window()
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  this->window = SDL_CreateWindow("SDL Demo", 0, 0, 1920, 1080,
                                  SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  if (!this->window)
    throw std::runtime_error(SDL_GetError());

  this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
  assert(this->renderer);
  this->debugGui.reset(new DebugGui(this->window, this->renderer));

  this->baseWidget.reset(new Widget(Widget::Position(100, 100), Widget::Color(30, 30, 30)));
  *this->baseWidget
    << (widget(Widget::Color(255, 0, 0), Widget::Layout::Vertical)
      << widget(Widget::Dimensions(20, 20), Widget::Color(255, 150, 50))
      << widget(Widget::Dimensions(20, 20), Widget::Color(255, 150, 50)))
    << widget(Widget::Dimensions(20, 20), Widget::Color(0, 255, 0))
    << widget(Widget::Dimensions(20, 20), Widget::Color(0, 0, 255))
    << widget(Widget::Dimensions(20, 20), Widget::Color(0, 255, 255))
    << widget(Widget::Dimensions(20, 20), Widget::Color(255, 0, 255));
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

  this->getDebugGui().draw(state, *this);


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

  SDL_Rect rect = {int(this->getWidth() / 2.0f - 200 / 2.0f + state.position.x),
                   int(this->getHeight() / 2.0f - 200 / 2.0f + state.position.y), 200, 200};
  SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(this->renderer, &rect);

  // Draw GUI on top of playfield

  this->baseWidget->applyLayout();
  this->baseWidget->draw(this->renderer);

  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(this->renderer);
}

bool Window::handleEvent(SDL_Event& event)
{
  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    this->needsResize = true;
  return this->getDebugGui().handleEvent(event);
}
