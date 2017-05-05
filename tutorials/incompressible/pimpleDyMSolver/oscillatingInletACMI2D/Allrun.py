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

# Code name and version
code = 'Caelus'
version = Caelus.PROJECT_VER

# Starting up the meshing and solving
print "*******************************************************"
print "Starting %s %s simulation of oscillatingInletACMI2D" % (code, version)
print "*******************************************************"

if sys.platform == 'win32':
   pltfrm = True
else:
   pltfrm = False

# Cleaning up the case
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Deleting 0 directory
if os.path.exists('0'):
	shutil.rmtree('0')

# Executing BlockMesh utility
print "Executing blockMesh"
run = subprocess.Popen(['caelus.py', '-l', 'blockMesh'], shell=pltfrm)
run.wait()
run = None

# Executing topoSet utility
print "Executing topoSet"
run = subprocess.Popen(['caelus.py', '-l', 'topoSet', '-constant'], shell=pltfrm)
run.wait()
run = None

# Executing createBaffles utility
print "Executing createBaffles"
run = subprocess.Popen(['caelus.py', '-l', 'createBaffles', '-overwrite'], shell=pltfrm)
run.wait()
run = None

# Executing createPatch utility
print "Executing createPatch"
run = subprocess.Popen(['caelus.py', '-l', 'createPatch', '-overwrite'], shell=pltfrm)
run.wait()
run = None


shutil.copytree('0.org', '0')

# Executing pimpleDyMSolver
print "Executing pimpleDyMSolver"
run = subprocess.Popen(['caelus.py', '-l', 'pimpleDyMSolver'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('oscillatingInletACMI2D.foam', 'a').close()
