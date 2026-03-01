#include "Label.hpp"

Label::Label(float x, float y, const std::string& text, const asw::Font& font, asw::Color color)
    : text(text)
    , label_font(font)
    , color(color)
{
    transform.set_position(x, y);
    z_index = 1;
}

void Label::draw()
{
    asw::draw::text(label_font, text, transform.position, color);
}
