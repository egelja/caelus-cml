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

# Code name and version
code = 'Caelus'
version = Caelus.PROJECT_VER

# Starting up the meshing and solving
print "**********************************"
print "Starting %s %s simulation" % (code, version)
print "**********************************"

if sys.platform == 'win32':
   pltfrm = True
else:
   pltfrm = False

# Cleaning up the case
os.system('caelus-cleanCase.py')

# Remove fields
os.chdir("0")
try:
  os.remove("Ma")
except OSError:
  pass
os.chdir("..")

# Executing pimpleSolver
print "Executing cpbnsLTS solver"
run = subprocess.Popen(['caelus.py', '-l', 'cpbnsLTS'], shell=pltfrm)
run.wait()
run = None

# Calculating Mach number
print "Calculating Mach number"
run = subprocess.Popen(['caelus.py', '-l', 'Mach'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('RAE2822.foam', 'a').close()
