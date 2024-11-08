#include <Commands/HandleListCommands.h>
#include <Commands/HandleHelp.h>
#include <stdlib.h>
#include <iostream>

void HandleHelp() {
  std::cout << "Welcome to Nova package manager " << version " by " << author  " with license " << license << std::endl;
  std::cout << "List of commands:" << std::endl;
  HandleListCommands();
  exit(0);
}
