#pragma once

#include <asw/asw.h>
#include <string>

class Button : public asw::game::GameObject {
 public:
  // Constructor
  Button(float x, float y, const std::string& text, const asw::Font& font);

  bool clicked() const;

  void update(float deltaTime) override;

  // Draw
  void draw() override;

 private:
  bool hovering = false;

  std::string text;

  // Font
  asw::Font button_font = nullptr;
};
