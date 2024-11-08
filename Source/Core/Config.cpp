#include <iostream>
#include <fstream>
#include <Core/Config.h>

struct Config *config = nullptr;

bool FileExists(const std::string& FileName) {
  std::ifstream File(FileName);
  return File.good();
}

int InitConfig() {
    if (config == nullptr) {
        if (!FileExists(ConfigPath)) {
            std::cout << "Error: no config found at " << ConfigPath << std::endl;
            exit(1);
        } else {
            config = new Config(ConfigPath);
            if (config == nullptr) {
                std::cout << "Error: failed to allocate memory for config" << std::endl;
                exit(1);
            }
        }
    }
    return 0;
}

void CleanConfig() {
  delete config;
  config = nullptr;
}
