#pragma once

#include <asw/asw.h>
#include <map>
#include <string>

class SampleRegistry {
 public:
  static void loadSample(const std::string& key, const std::string& path);
  static asw::Sample getSample(const std::string& key);

 private:
  static std::map<std::string, asw::Sample> samples;
};
