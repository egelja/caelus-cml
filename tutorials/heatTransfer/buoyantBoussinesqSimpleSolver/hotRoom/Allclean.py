#!/usr/bin/python

# ---------------------------------------------------------------------------
# Caelus 5.04
# Web:   www.caelus-cml.com
# ---------------------------------------------------------------------------

# Importing the required modules for Python
import subprocess
import sys
import os
import glob
import shutil

# Starting up the meshing and solving
print "**********************************"
print "Cleaning hotRoom tutorial         "
print "**********************************"

# Cleaning up the case
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Remove T field is it exists
os.chdir("0")
try:
  os.remove("T.gz")
except OSError:
  pass
