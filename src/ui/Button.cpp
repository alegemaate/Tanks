#include "Button.hpp"

using namespace asw::input;

constexpr float BUTTON_PADDING = 20.0F;

// Constructor
Button::Button(float x, float y, const std::string& text, const asw::Font& font)
    : text(text), button_font(font) {
  transform.setPosition(x, y);

  auto text_size = asw::util::getTextSize(button_font, text);
  transform.setSize(text_size.x + BUTTON_PADDING, text_size.y + BUTTON_PADDING);

  zIndex = 1;
}

// Update
void Button::update(float _deltaTime) {
  hovering = transform.contains(asw::input::mouse.x, asw::input::mouse.y);
}

// True if clicked
bool Button::clicked() const {
  return hovering &&
         asw::input::wasButtonPressed(asw::input::MouseButton::LEFT);
}

// Draw
void Button::draw() {
  // Backdrop
  const int c_element = hovering ? 220 : 200;

  asw::draw::rectFill(transform,
                      asw::util::makeColor(c_element, c_element, c_element));
  asw::draw::rect(transform, asw::util::makeColor(0, 0, 0));

  // Text
  const float padding = BUTTON_PADDING / 2.0F;
  asw::draw::text(button_font, text,
                  transform.position + asw::Vec2<float>(padding, padding),
                  asw::util::makeColor(0, 0, 0));
}
