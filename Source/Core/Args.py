def HandleArgs(Arg):
  match Arg[1]:
    case "install" | "fetch" | "get":
      if len(Arg) <= 2:
        print("Error: No package(s) specified")
        exit(1)
      else:
        from Core import Install
        Install.QuikResolve(Arg[2:])
    case "help":
        from Commands import HandleHelp
        HandleHelp.HandleHelp()
    case "info":
      if len(Arg) > 3:
        print("Eror: can check info of only one package!")
        exit(1)
      if len(Arg) <= 2:
        print("Error: no packages provided!")
        exit(1) 
      from Core import Info
      Info.QueryInfo(Arg[2])
    case _:
      print(f"Unknown option {Arg[1]}")
      exit(1)

if __name__ == '__main__':
  print("This is not meant to be run!")
  exit(1)

