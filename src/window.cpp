#include "window.hpp"
#include <cassert>
#include <format>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <iostream>
#include <SDL2/SDL.h>

Window::Window(const char* title)
{
  SDL_SetHint(SDL_HINT_EVENT_LOGGING, "1");
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("Failed to initialize SDL");

  this->window =
      SDL_CreateWindow(title, 0, 0, this->getWidth(), this->getHeight(), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  assert(this->window);

  this->logEnv();
  this->logDisplayInfo();

  this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  assert(this->renderer);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(window, this->renderer);
  ImGui_ImplSDLRenderer2_Init(this->renderer);
}

Window::~Window()
{
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Window::draw()
{
  if (this->needsResize)
  {
    this->needsResize = false;
    SDL_GetWindowSize(this->window, &this->dimensions.width, &this->dimensions.height);
  }

  // Start the Dear ImGui frame
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  bool showDemoWindow = true;
  ImGui::ShowDemoWindow(&showDemoWindow);

  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO();
  SDL_RenderSetScale(this->renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderClear(this->renderer);

  int width = this->getWidth() / 3;
  int height = this->getHeight() / 3;
  SDL_Rect rect = {this->getWidth() / 2 - width / 2, this->getHeight() / 2 - height / 2, width, height};
  SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(this->renderer, &rect);

  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(this->renderer);
}

void Window::handleEvent(SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    this->needsResize = true;
}

void Window::logEnv()
{
  printf(
      "Environment: DISPLAY=%s WAYLAND_DISPLAY=%s DESKTOP_SESSION=%s XDG_SESSION_DESKTOP=%s XDG_CURRENT_DESKTOP=%s\n",
      getenv("DISPLAY"), getenv("WAYLAND_DISPLAY"), getenv("DESKTOP_SESSION"), getenv("XDG_SESSION_DESKTOP"),
      getenv("XDG_CURRENT_DESKTOP"));
}

void Window::logDisplayInfo()
{
  int numberOfAdapters = SDL_GetNumVideoDisplays();
  printf("Available displays: %d\n", numberOfAdapters);
  for (int i = 0; i < numberOfAdapters; i++)
  {
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(i, &rect) == 0)
    {
      const char* name = SDL_GetDisplayName(i);
      printf("%s\n", name);
    }
  }
}
