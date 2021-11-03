#ifndef SRC_SYSTEM_IMAGEREGISTRY_H_
#define SRC_SYSTEM_IMAGEREGISTRY_H_

#include <allegro.h>
#include <map>
#include <string>

class ImageRegistry {
 public:
  static void loadImage(const std::string& key, const std::string& path);
  static BITMAP* getImage(const std::string& key);

 private:
  static std::map<std::string, BITMAP*> images;
};

#endif  // SRC_SYSTEM_IMAGEREGISTRY_H_