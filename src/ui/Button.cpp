#include "Button.hpp"

using namespace asw::input;

constexpr float BUTTON_PADDING = 20.0F;

// Constructor
Button::Button(float x, float y, const std::string& text, const asw::Font& font)
    : text(text)
    , button_font(font)
{
    transform.set_position(x, y);

    auto text_size = asw::util::get_text_size(button_font, text);
    transform.set_size(text_size.x + BUTTON_PADDING, text_size.y + BUTTON_PADDING);

    z_index = 1;
}

// Update
void Button::update(float _dt)
{
    hovering = transform.contains(asw::input::mouse.position);
}

// True if clicked
bool Button::clicked() const
{
    return hovering && asw::input::get_mouse_button_down(asw::input::MouseButton::Left);
}

// Draw
void Button::draw()
{
    // Backdrop
    const int c_element = hovering ? 220 : 200;

    asw::draw::rect_fill(transform, asw::Color(c_element, c_element, c_element));
    asw::draw::rect(transform, asw::Color(0, 0, 0));

    // Text
    const float padding = BUTTON_PADDING / 2.0F;
    asw::draw::text(button_font, text, transform.position + asw::Vec2<float>(padding, padding),
        asw::Color(0, 0, 0));
}
