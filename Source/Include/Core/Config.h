#include <string>
#include <vector>
#include <stdlib.h>
#include <INIReader.h>

#define ConfigPath "/etc/npkg/Config"

struct Config {
  INIReader reader;

  Config(const std::string& path) : reader(path) {}

  std::string ResolveConfig(const std::string& section, const std::string& key) {
    std::string value = reader.Get(section, key, "");
    if (value.empty()) {
      std::cout << "Error: No value found for [" << section << "] " << key << std::endl;
      exit(1);
    }
    return value;
  }
};

extern Config *config;


bool FileExists(const std::string& fileName);
int InitConfig();
void CleanConfig();

