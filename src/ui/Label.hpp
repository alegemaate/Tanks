#pragma once

#include <asw/asw.h>
#include <string>

class Label : public asw::game::GameObject {
public:
    Label(float x, float y, const std::string& text, const asw::Font& font, asw::Color color);

    void draw() override;

private:
    std::string text;
    asw::Font label_font = nullptr;
    asw::Color color;
};
