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
os.system('caelus-clearPolyMesh.py')

# Copy of T field
shutil.copy2('0/T.org', '0/T')

# Executing BlockMesh utility
print "Executing blockMesh"
run = subprocess.Popen(['caelus.py', '-l', 'blockMesh'], shell=pltfrm)
run.wait()
run = None

# Executing setFields
print "Executing setFields"
run = subprocess.Popen(['caelus.py', '-l', 'setFields'], shell=pltfrm)
run.wait()
run = None

# Executing buoyantSimpleSolver
print "Executing buoyantSimpleSolver"
run = subprocess.Popen(['caelus.py', '-l', 'buoyantSimpleSolver'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('hotRoom.foam', 'a').close()
