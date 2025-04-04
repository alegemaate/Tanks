#ifndef SRC_UI_BUTTON_H_
#define SRC_UI_BUTTON_H_

#include <asw/asw.h>
#include <string>

class Button {
 public:
  // Constructor
  Button(int x, int y, const std::string& text, asw::Font button_font);

  Button() = default;

  int getWidth() const { return width + padding_x * 2; }
  int getHeight() const { return height + padding_y * 2; }

  bool setFont(asw::Font font);

  bool clicked() const;
  void update();

  // Draw
  void draw() const;

 private:
  // Variables
  int x = 0;
  int y = 0;
  int width{};
  int height{};
  int padding_x = 10;
  int padding_y = 10;
  std::string text;

  bool hovering = false;
  bool visible = true;

  // Font
  asw::Font button_font = nullptr;
};

#endif  // SRC_UI_BUTTON_H_
