#pragma once

#include <asw/asw.h>
#include <map>
#include <string>

class ImageRegistry {
 public:
  static void loadImage(const std::string& key, const std::string& path);
  static asw::Texture getImage(const std::string& key);

 private:
  static std::map<std::string, asw::Texture> images;
};
