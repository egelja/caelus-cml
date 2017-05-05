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
print "Cleaning tutorial: floatingObject"

# Cleaning up the case
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Remove 0 directory if it exists
try:
  shutil.rmtree("0")
except OSError:
  pass

# Remove dynamicMeshDict field is it exists
os.chdir("constant")
try:
  os.remove("dynamicMeshDict")
except OSError:
  pass
os.chdir("..")
