import os
import configparser

Config = None

def InitConfig():
  global Config
  if Config is None:
    if not os.path.isfile(f"{os.getenv('HOME')}/.npkg/Config"):
      print("Error: no config found!")
      exit(1)
    else:
      Config = configparser.ConfigParser()
      Config.read(f"{os.getenv("HOME")}/.npkg/Config")

def ResolveConfig(Section, Option):
  try:
    return Config.get(Section, Option)
  except (configparser.NoOptionError, configparser.NoSectionError):
    print(f"Error: missing required configuration '{Option}' in section '{Section}'.")
    exit(1)

def ResolveConfigWithFallbackKey(Section, Option, FallBackKey):
  try:
    return Config.get(Section, Option)
  except (configparser.NoOptionError, configparser.NoSectionError):
    return FallBackKey

