#pragma once
#include <memory>
#include <string_view>

struct ImGuiIO;
struct ImVec2;
struct SDL_Renderer;
struct SDL_Window;
struct State;
union SDL_Event;

struct Dimensions
{
  int width;
  int height;
};

class Window
{
public:
  Window(const char* title);
  ~Window();
  void draw(State& state);
  ImGuiIO& handleEvent(SDL_Event& event);
  int getWidth() const { return this->dimensions.width; }
  int getHeight() const { return this->dimensions.height; }

private:
  void logEnv();
  void logDisplayInfo();

  SDL_Window* window;
  SDL_Renderer* renderer;

  Dimensions dimensions = {1920, 1080};

  bool needsResize = false;
  bool showDemoWindow = false;
};
