#!/usr/bin/python
# ---------------------------------------------------------------------------
# Caelus 6.04
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
print "Cleaning motorBikeLES tutorial    "
print "**********************************"

# Cleaning up the case
os.system('caelus-cleanCase.py')
if os.path.exists('constant/polyMesh'):
   shutil.rmtree('constant/polyMesh')

# Remove fields
os.chdir("0")
try:
  os.remove("fr1.gz")
except OSError:
  pass
try:
  os.remove("nut.gz")
except OSError:
  pass
try:
  os.remove("nuTilda.gz")
except OSError:
  pass
try:
  os.remove("p.gz")
except OSError:
  pass
try:
  os.remove("phi.gz")
except OSError:
  pass
try:
  os.remove("U.gz")
except OSError:
  pass
os.chdir("..")
