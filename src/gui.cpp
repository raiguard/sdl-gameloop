#include "gui.hpp"
#include "state.hpp"
#include "window.hpp"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

Gui::Gui(SDL_Window* window, SDL_Renderer* renderer)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  this->io = &ImGui::GetIO();
  this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);
}

Gui::~Gui()
{
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void Gui::update(State& state)
{
  // Start the Dear ImGui frame
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (state.showDemoWindow)
    ImGui::ShowDemoWindow(&state.showDemoWindow);

  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;
  ImGui::Begin("Debug overlay", nullptr, flags);
  ImGui::Text("%.1f FPS (%.3f ms/frame)", this->io->Framerate, 1000.0f / this->io->Framerate);
  ImGui::Text("Position: (%0.1f, %0.1f)", state.position.x, state.position.y);
  if (ImGui::Button("Show demo window"))
    state.showDemoWindow = !state.showDemoWindow;
  ImGui::SameLine();
  if (ImGui::Button("Reset position"))
    state.position = ImVec2();
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2{state.screenSize.width / 2.0f, float(state.screenSize.height)}, true, ImVec2{0.5, 1});
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
}

void Gui::handleEvent(SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
}
