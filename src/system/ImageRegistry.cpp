#include "ImageRegistry.hpp"

std::map<std::string, asw::Texture> ImageRegistry::images;

void ImageRegistry::loadImage(const std::string& imageKey,
                              const std::string& path) {
  asw::Texture image = asw::assets::loadTexture(path);

  if (!image) {
    asw::util::abortOnError("Cannot find image " + path +
                            "\nPlease check your files and try again");
  }

  ImageRegistry::images[imageKey] = image;
}

asw::Texture ImageRegistry::getImage(const std::string& imageKey) {
  auto image = ImageRegistry::images[imageKey];

  if (!image) {
    asw::util::abortOnError("Cannot find image " + imageKey);
  }

  return image;
}
