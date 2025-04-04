#include "SampleRegistry.hpp"

std::map<std::string, asw::Sample> SampleRegistry::samples;

void SampleRegistry::loadSample(const std::string& key,
                                const std::string& path) {
  SampleRegistry::samples[key] = asw::assets::loadSample(path);
}

asw::Sample SampleRegistry::getSample(const std::string& key) {
  auto image = SampleRegistry::samples[key];

  if (!image) {
    asw::util::abortOnError("Cannot find image " + key);
  }

  return image;
}
