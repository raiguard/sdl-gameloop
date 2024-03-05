#include "widget.hpp"
#include <SDL_rect.h>
#include <imgui.h>

WidgetPtr& WidgetPtr::operator<<(std::shared_ptr<Widget> other)
{
  *this->get() << other;
  return *this;
}

Widget::Widget(Color color, Layout layout)
  : color(color), layout(layout)
{}

Widget::Widget(Position position, Color color, Layout layout)
  : position(position), color(color), layout(layout)
{}

Widget::Widget(Dimensions dimensions, Color color, Layout layout)
 : dimensions(dimensions), color(color), layout(layout)
{}

void Widget::applyLayout()
{
  if (this->children.empty())
    return;

  this->dimensions = Dimensions();
  Position childPosition = this->position;
  childPosition.x += 10;
  childPosition.y += 10;
  for (auto& child : this->children)
  {
    child->position = childPosition;
    child->applyLayout();
    switch (this->layout)
    {
    case Layout::Horizontal:
      childPosition.x += child->dimensions.w;
      childPosition.x += 10; // Spacing
      break;
    case Layout::Vertical:
      childPosition.y += child->dimensions.h;
      childPosition.y += 10; // Spacing
      break;
    }
    this->dimensions.w = std::max(this->dimensions.w, child->position.x + child->dimensions.w);
    this->dimensions.h = std::max(this->dimensions.h, child->position.y + child->dimensions.h);
  }
  this->dimensions.w -= this->position.x - 10;
  this->dimensions.h -= this->position.y - 10;
}

void Widget::draw(SDL_Renderer* renderer) const
{
  // SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
  // SDL_Rect rect = {this->position.x, this->position.y, this->dimensions.w, this->dimensions.h};
  // SDL_RenderFillRect(renderer, &rect);

  // for (auto& child : this->children)
  //   child->draw(renderer);
}
