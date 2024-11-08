import time
from Core import Config
from Core import NPE
import subprocess
import os

Repo = Config.ResolveConfig("Global", "Repo")
InstallDIR = Config.ResolveConfig("Global", "InstallDIR")
PackagesDIR = Config.ResolveConfig("Global", "PackagesDIR")
CacheDIR = Config.ResolveConfig("Global", "CacheDIR")

DownloadPath = CacheDIR

if not os.path.exists(InstallDIR):
        os.mkdir(InstallDIR)

def QuikResolve(Arg):
    from hashlib import sha256
    StartTime = time.time()
    for Packages in Arg:
        FirstTwo = Packages[:2]
        if not os.path.isdir(f"{PackagesDIR}/{FirstTwo}/{Packages}"):
            print(f"Error: package {Packages} not found")
            exit(1)
    ResolvedTime = time.time() - StartTime
    print(f"Resolved package(s) in {ResolvedTime} seconds")
    InstallPackages(Arg)

def DownloadPackage(URL, Path):
  import requests
  from tqdm import tqdm
  Response = requests.get(URL, stream=True)
  Response.raise_for_status()
  TotalSize = int(Response.headers.get('content-length', 0)) or 25000000
  try:
    with open(Path, 'wb') as PackagedFile, tqdm(
      total=TotalSize, unit='B', unit_scale=True, desc=os.path.basename(Path)
      ) as ProgressBar:
      for chunk in Response.iter_content(chunk_size=32768):
        PackagedFile.write(chunk)
        ProgressBar.update(len(chunk))

  except requests.exceptions.Connection Error:
    print(f"Error: unable to reach repo {Repo}")
    exit(1)
  except requests.exceptions.Timeout:
    print("Error: request timed out for repo {Repo}")
    exit(1)
  except requests.exceptions.RequestException as Error:
    print(f"Error: {Error}")
    exit(1)

def GetPackage(Package, Repo):
    if not Repo.startswith(('http://', 'https://')):
        Repo = 'http://' + Repo
    if os.path.isfile(os.path.join(CacheDIR, Package + '.npe')):
        print(f"Info: using already cached package for {Package}")
        return
    else:
      PackageURL = Repo + f'/{Package}.npe'
      SignatureURL = Repo + f'/{Package}.sig'
      DownloadPackage(SignatureURL, f'{DownloadPath}/{Package}.sig')
      DownloadPackage(PackageURL, f'{DownloadPath}/{Package}.npe')
            

def InstallPackage(Package):
    try:
      NPE.VerifySig(f'{CacheDIR}/{Package}')
      Command = f'lz4 -dc < {CacheDIR}/{Package}.npe | tar -xf - -C {InstallDIR}'
      Process = subprocess.run(Command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)   
      if Process.returncode != 0:
        print(f"Error: couldn't extract package '{Package}': {Process.stderr.decode()}")
        exit(1)

    except Exception as e:
        print(f"Error: couldn't extract package '{Package}': {e}")
        exit(1)

def InstallPackages(Packages):
    for Package in Packages:
        GetPackage(Package, Repo)
        InstallPackage(Package)

def ProgressBar(itr, tot, length=50):
  percent = (iteration / total) * 100
  arrow = '=' * int(length * (iteration / total) - 1)
  spaces = ' ' * (length - len(arrow) - 2)
  sys.stdout.write(f'\r[{arrow}<>{spaces}] {percent:.2f}%')
  sys.stdout.flush()
