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

  this->window = SDL_CreateWindow(title, 0, 0, this->getWidth(), this->getHeight(),
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
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
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Window::draw(ImVec2& position)
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

  ImGuiIO& io = ImGui::GetIO();

  if (this->showDemoWindow)
    ImGui::ShowDemoWindow(&this->showDemoWindow);

  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;
  ImGui::Begin("Debug overlay", nullptr, flags);
  ImGui::Text("%.1f FPS (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
  if (ImGui::Button("Show demo"))
    this->showDemoWindow = !this->showDemoWindow;
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2{this->getWidth() / 2.0f, float(this->getHeight())}, true, ImVec2{0.5, 1});
  ImGui::Begin("Hotbar", nullptr,
               ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});

  ImGui::SameLine(0, 8);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::SameLine(0, 4);
  ImGui::Button("##", ImVec2{40, 40});
  ImGui::End();

  ImGui::Render();
  SDL_RenderSetScale(this->renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderClear(this->renderer);

  int width = this->getWidth() / 3;
  int height = this->getHeight() / 3;
  SDL_Rect rect = {this->getWidth() / 2 - width / 2, int(this->getHeight() / 2.0f - height / 2.0f + position.y), width, height};
  SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(this->renderer, &rect);

  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(this->renderer);
}

ImGuiIO& Window::handleEvent(SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    this->needsResize = true;
  return ImGui::GetIO();
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
