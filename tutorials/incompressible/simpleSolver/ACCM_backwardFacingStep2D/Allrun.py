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

# Executing simpleSolver
print "Executing simpleSolver"
run = subprocess.Popen(['caelus.py', '-l', 'simpleSolver'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('ACCM_backwardFacingStep2D.foam', 'a').close()
