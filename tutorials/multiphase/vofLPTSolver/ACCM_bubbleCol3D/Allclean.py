#!/usr/bin/python

# ---------------------------------------------------------------------------
# Caelus 5.10
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
print "**********************************"
print "Cleaning bubble column tutorial   "
print "**********************************"

# Cleaning up the case
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Remove alpha1 field is it exists
os.chdir("0")
try:
  os.remove("alpha1.gz")
except OSError:
  pass
os.chdir("..")
