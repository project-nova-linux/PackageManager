import subprocess
import lz4.frame
import gnupg
import os

def VerifySig(Package):
  gpg = gnupg.GPG()

  with open(Package + '.sig', 'rb') as File:
    Verified = gpg.verify_file(File, Package + '.npe')
  
  if not Verified:
    print("Error: invalid signature")
    exit(1)

