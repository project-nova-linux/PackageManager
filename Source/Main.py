import sys

def Main():
  Arg = sys.argv
  ArgLen = len(sys.argv)
  if ArgLen <= 1:
    from Commands import HandleHelp
    HandleHelp.HandleHelp()
  else:
    from Core.Config import InitConfig 
    InitConfig()
    from Core import Args
    Args.HandleArgs(Arg)
  
if __name__ == '__main__':
  Main()
else:
  print("This is not meant to be imported!")
  exit(1)
