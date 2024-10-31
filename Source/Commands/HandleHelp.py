version = "0.0.1-alpha"
author = "Ishan Neupane"
license = "MIT"

from Commands import HandleListCommands

def HandleHelp():
  print(f"Welcome to Nova package manager {version} by {author} licensed with {license}")
  print("List of commands:")
  HandleListCommands.HandleListCommands()
  exit(0)

if __name__ == '__main__':
  print("This is not meant to be run!")
  exit(1)
