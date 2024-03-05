#include <memory>
#include <vector>

struct SDL_Renderer;

class Widget;
struct WidgetPtr: std::shared_ptr<Widget>
{
  using Super = std::shared_ptr<Widget>;
public:
  WidgetPtr(Super&& self) : Super(self) {}
  virtual WidgetPtr& operator<<(std::shared_ptr<Widget> other);
};

class Widget
{
public:
  struct Color
  {
  public:
    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
      : r(r), g(g), b(b), a(a)
    {}

    uint8_t r, g, b, a;
  };

  enum class Layout
  {
    Horizontal,
    Vertical,
  };

  struct Position
  {
    Position() = default;
    Position(int x, int y) : x(x), y(y) {}
    int x = 0;
    int y = 0;
  };

  struct Dimensions
  {
    Dimensions() = default;
    Dimensions(int w, int h) : w(w), h(h) {}
    int w = 0;
    int h = 0;
  };

  Widget(Color color, Layout layout = Layout::Horizontal);
  Widget(Position position, Color color, Layout layout = Layout::Horizontal);
  Widget(Dimensions dimensions, Color color, Layout layout = Layout::Horizontal);
  virtual void applyLayout();
  virtual void draw(SDL_Renderer* renderer) const;

  virtual Widget& operator<<(std::shared_ptr<Widget> other) { this->children.push_back(std::move(other)); return *this; }

private:
  Position position;
  Dimensions dimensions;

  Layout layout = Layout::Horizontal;
  Color color = Color(255, 255, 255);

  std::vector<std::shared_ptr<Widget>> children;
};

inline WidgetPtr widget(Widget::Color color, Widget::Layout layout = Widget::Layout::Horizontal)
{
  return WidgetPtr(std::make_shared<Widget>(color, layout));
}

inline WidgetPtr widget(Widget::Dimensions dimensions, Widget::Color color, Widget::Layout layout = Widget::Layout::Horizontal)
{
  return WidgetPtr(std::make_shared<Widget>(dimensions, color, layout));
}

// this->baseWidget.reset(new Widget(Widget::Position(100, 100), Widget::Color(30, 30, 30)));
// *this->baseWidget
//   << (widget(Widget::Color(255, 0, 0), Widget::Layout::Vertical)
//     << widget(Widget::Dimensions(20, 20), Widget::Color(255, 150, 50))
//     << widget(Widget::Dimensions(20, 20), Widget::Color(255, 150, 50)))
//   << widget(Widget::Dimensions(20, 20), Widget::Color(0, 255, 0))
//   << widget(Widget::Dimensions(20, 20), Widget::Color(0, 0, 255))
//   << widget(Widget::Dimensions(20, 20), Widget::Color(0, 255, 255))
//   << widget(Widget::Dimensions(20, 20), Widget::Color(255, 0, 255));
