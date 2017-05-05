#!/usr/bin/python
# ---------------------------------------------------------------------------
# Caelus 6.10
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
print "Cleaning RAE2882 tutorial         "
print "**********************************"

# Cleaning up the case
os.system('caelus-cleanCase.py')

# Remove fields
os.chdir("0")
try:
  os.remove("Ma")
except OSError:
  pass
os.chdir("..")

