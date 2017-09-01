#!/usr/bin/python
# ---------------------------------------------------------------------------
# Caelus 7.04
# Web:   www.caelus-cml.com
# ---------------------------------------------------------------------------

# Importing the required modules for Python
import subprocess
import sys
import os
import glob
import shutil
import Caelus

# Starting up the meshing and solving
print "Cleaning tutorial: waveFlume"

# Cleaning up the case
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Remove waveProperties if it exists
os.chdir("constant")
try:
  os.remove("waveProperties")
except OSError:
  pass
os.chdir("..")

# Remove alpha1 field if it exists
os.chdir("0")
try:
  os.remove("alpha1")
except OSError:
  pass
os.chdir("..")

# Remove pressure field if it exists
os.chdir("0")
try:
  os.remove("p_rgh")
except OSError:
  pass
os.chdir("..")

# Remove velocity field if it exists
os.chdir("0")
try:
  os.remove("U")
except OSError:
  pass
os.chdir("..")
