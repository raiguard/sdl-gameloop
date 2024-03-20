#include "debuggui.hpp"
#include "state.hpp"
#include "window.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

DebugGui::DebugGui(SDL_Window* window, SDL_GLContext context)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, context);
  ImGui_ImplOpenGL3_Init();
}

DebugGui::~DebugGui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void DebugGui::draw(State& state, Window& window)
{
  if (state.showDemoWindow)
    ImGui::ShowDemoWindow(&state.showDemoWindow);

  ImGuiIO& io = ImGui::GetIO();
  ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Video driver: %s", SDL_GetCurrentVideoDriver());
  ImGui::Text("Render: %.1f FPS (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
  if (ImGui::Checkbox("Use vsync", &window.useVsync))
    window.updateVsync = true;
  ImGui::Text("True resolution: %dx%d", window.getWidth(), window.getHeight());
  auto scaledSize = window.getScaledSize();
  ImGui::Text("Scaled resolution: %dx%d", scaledSize.first, scaledSize.second);
  ImGui::Text("Display density: %fx", double(window.getWidth()) / scaledSize.first);
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
