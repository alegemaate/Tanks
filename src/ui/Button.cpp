#include "Button.hpp"

using namespace asw::input;

// Constructor
Button::Button(int x, int y, const std::string& text, asw::Font button_font)
    : x(x), y(y), text(text) {
  setFont(button_font);
}

// Set new font
bool Button::setFont(asw::Font font) {
  this->button_font = font;
  if (button_font != nullptr) {
    auto textSize = asw::util::getTextSize(button_font, text);
    this->width = textSize.x;
    this->height = textSize.y;
    return true;
  }
  return false;
}

// Update
void Button::update() {
  hovering = asw::input::mouse.x > x && asw::input::mouse.x < x + getWidth() &&
             asw::input::mouse.y > y && asw::input::mouse.y < y + getHeight();
}

// True if clicked
bool Button::clicked() const {
  return hovering &&
         asw::input::wasButtonPressed(asw::input::MouseButton::LEFT);
}

// Draw
void Button::draw() const {
  if (!visible) {
    return;
  }

  // Backdrop
  const int c_element = hovering ? 220 : 200;

  asw::draw::rectFill(
      asw::Quad<float>(x, y, width + padding_x * 2, height + padding_y * 2),
      asw::util::makeColor(c_element, c_element, c_element));
  asw::draw::rect(
      asw::Quad<float>(x, y, width + padding_x * 2, height + padding_y * 2),
      asw::util::makeColor(0, 0, 0));

  // Text
  if (button_font != nullptr) {
    asw::draw::text(button_font, text,
                    asw::Vec2<float>(x + padding_x, y + padding_y),
                    asw::util::makeColor(0, 0, 0));
  }
}
