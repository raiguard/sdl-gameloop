#include "debuggui.hpp"
#include "widget.hpp"
#include "window.hpp"
#include "state.hpp"
#include <cassert>
#include <SDL2/SDL.h>
#include <format>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <thread>

Window::Window()
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  SDL_GL_LoadLibrary(nullptr);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  this->window = SDL_CreateWindow("SDL Demo", 0, 0, 1920, 1080,
                                  SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
  if (!this->window)
    throw std::runtime_error(SDL_GetError());

  this->context = SDL_GL_CreateContext(this->window);
  assert(this->context);
  this->debugGui.reset(new DebugGui(this->window, this->context));

  gladLoadGLLoader(SDL_GL_GetProcAddress);
  std::cout << std::format("OpenGL version: {}", (const char*)(glGetString(GL_VERSION))) << std::endl;

  glGenBuffers(1, &this->buffer);
  glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(this->positions), this->positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

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
  // GUI

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  this->getDebugGui().draw(state, *this);

  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO();
  if (this->updateVsync)
  {
    this->updateVsync = false;
    SDL_GL_SetSwapInterval(this->useVsync ? 1 : 0);
  }

  // Playfield

  glViewport(0, 0, this->getWidth(), this->getHeight());
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Draw GUI on top of playfield

  this->baseWidget->applyLayout();
  // this->baseWidget->draw(this->renderer);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  SDL_GL_SwapWindow(this->window);
}

bool Window::handleEvent(SDL_Event& event)
{
  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    this->needsResize = true;
  return this->getDebugGui().handleEvent(event);
}
