#include "debuggui.hpp"
#include "widget.hpp"
#include "window.hpp"
#include "state.hpp"
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

Window::Window()
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  this->window = SDL_CreateWindow("SDL Demo", 0, 0, 1920, 1080,
                                  SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
  if (!this->window)
    throw std::runtime_error(SDL_GetError());

  this->context = SDL_GL_CreateContext(this->window);
  assert(this->context);
  this->debugGui.reset(new DebugGui(this->window, this->context));

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
  SDL_GL_DeleteContext(this->context);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Window::prepare()
{
  if (this->needsResize)
  {
    this->needsResize = false;
    SDL_GetWindowSize(this->window, &this->width, &this->height);
  }
}

void Window::draw(State& state)
{
  // TODO: Double buffering if that is needed for repro
  // GUI

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  this->getDebugGui().draw(state, *this);

  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO();
  // SDL_RenderSetScale(this->renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  if (this->updateVsync)
  {
    this->updateVsync = false;
    SDL_GL_SetSwapInterval(this->useVsync ? 1 : 0);
  }

  // Playfield

  glViewport(0, 0, this->getHeight(), this->getWidth());
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw GUI on top of playfield

  this->baseWidget->applyLayout();
  // this->baseWidget->draw(this->renderer);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(this->window);
}

bool Window::handleEvent(SDL_Event& event)
{
  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    this->needsResize = true;
  return this->getDebugGui().handleEvent(event);
}
