#include "debuggui.hpp"
#include "state.hpp"
#include "window.hpp"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

DebugGui::DebugGui(SDL_Window* window, SDL_Renderer* renderer)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);
}

DebugGui::~DebugGui()
{
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void DebugGui::draw(State& state, Window& window)
{
  if (state.showDemoWindow)
    ImGui::ShowDemoWindow(&state.showDemoWindow);

  ImGuiIO& io = ImGui::GetIO();
  ImGui::Begin("Debug");
  ImGui::Text("Render: %.1f FPS (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
  if (ImGui::Checkbox("Use vsync", &window.useVsync))
    window.updateVsync = true;
  ImGui::Text("Resolution: %dx%d", window.getWidth(), window.getHeight());
  ImGui::Text("Position: (%0.1f, %0.1f)", state.position.x, state.position.y);
  ImGui::Text("Counter: %lu", state.counter);
  if (ImGui::Button("Show demo window"))
    state.showDemoWindow = !state.showDemoWindow;
  ImGui::SameLine();
  if (ImGui::Button("Reset position"))
    state.position = ImVec2();
  ImGui::End();
}

bool DebugGui::handleEvent(SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
  return ImGui::GetIO().WantCaptureKeyboard;
}