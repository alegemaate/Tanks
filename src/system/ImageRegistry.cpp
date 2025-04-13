#include "ImageRegistry.hpp"

std::map<std::string, asw::Texture> ImageRegistry::images;

void ImageRegistry::loadImage(const std::string& key, const std::string& path) {
  ImageRegistry::images[key] = asw::assets::loadTexture(path);
}

asw::Texture ImageRegistry::getImage(const std::string& key) {
  auto image = ImageRegistry::images[key];

  if (!image) {
    asw::util::abortOnError("Cannot find image " + key);
  }

  return image;
}
