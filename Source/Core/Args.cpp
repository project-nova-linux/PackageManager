#include <Core/Args.h>
#include <Core/Install.h>
#include <Commands/HandleHelp.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <functional>
#include <stdlib.h>

void HandleInstall(int ArgLen, char *Arg[]) {
  if(ArgLen <= 2) {
    std::cout << "Error: No package(s) specified" << std::endl;
    exit(1);
  } else { 
    QuikResolve(ArgLen, Arg);
  }
}

int HandleArgs(int ArgLen, char *Arg[]) {
  std::unordered_map<std::string, std::function<void(int, char*[])>> Options = {
    {"install", [](int ArgLen, char *Arg[]) { HandleInstall(ArgLen, Arg); }},
    {"fetch", [](int ArgLen, char *Arg[]) { HandleInstall(ArgLen, Arg); }},
    {"get", [](int ArgLen, char *Arg[]) { HandleInstall(ArgLen, Arg); }},
    {"help", [](int ArgLen, char *Arg[]) { HandleHelp(); }}
  };
  auto it = Options.find(Arg[1]);
  if (it != Options.end()) {
    it->second(ArgLen, Arg);
  } else {
    std::cout << "Unknown option: " << Arg[1] << std::endl;
    exit(1);
  }
  return 0;
}
