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

# Deleting 0 directory
if os.path.exists('0'):
   shutil.rmtree('0')

shutil.copytree('0.org', '0')

# Executing blockMesh utility
print "Executing blockMesh"
run = subprocess.Popen(['caelus.py', '-l', 'blockMesh'], shell=pltfrm)
run.wait()
run = None

# Executing decomposePar
print "Decomposing the mesh"
run = subprocess.Popen(['caelus.py', '-l', 'decomposePar', '-force'], shell=pltfrm)
run.wait()
run = None

# Executing setFields
print "Initialising fields in parallel with setFields"
run = subprocess.Popen(['caelus.py', '-l', 'setFields', '-parallel'], shell=pltfrm)
run.wait()
run = None

# Executing explicitDBNSSolver
print "Executing explicitDBNSSolver in parallel"
run = subprocess.Popen(['caelus.py', '-l', 'explicitDBNSSolver', '-parallel'], shell=pltfrm)
run.wait()
run = None

# Executing reconstructPar
print "Reconstructing solution with reconstructPar"
run = subprocess.Popen(['caelus.py', '-l', 'reconstructPar', '-latestTime'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('2DRiemann800.foam', 'a').close()
