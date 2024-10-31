import os
from hashlib import sha256
from Core import Config

PackagesDIR = Config.ResolveConfig("Global", "PackagesDIR")

def QueryInfo(Package):
  FirstTwo = Package[:2]
  SHA256SUM = sha256(Package.encode('ascii')).hexdigest()
  if not os.path.exists(f'{PackagesDIR}/{FirstTwo}/{SHA256SUM}'):
    print(f"Error: package {Package} does not exist!")
    exit(1)
  if not os.path.isfile(f'{PackagesDIR}/{FirstTwo}/{SHA256SUM}/Info'):
    print(f"Error: cannot find info for package {Package}")
    exit(1)
  

