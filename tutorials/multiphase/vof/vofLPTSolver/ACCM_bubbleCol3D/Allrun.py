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
print "Cleaning case"
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Copy of alpha1 field
shutil.copy2('0/alpha1.org', '0/alpha1')

# Executing BlockMesh utility
print "Executing blockMesh"
run = subprocess.Popen(['caelus.py', '-l', 'blockMesh'], shell=pltfrm)
run.wait()
run = None

# Executing setSet
print "Executing setSet"
run = subprocess.Popen(['caelus.py', '-l', 'setSet', '-batch', 'system//makeFaceSet.setSet'], shell=pltfrm)
run.wait()
run = None

# Executing createPatch
print "Executing createPatch"
run = subprocess.Popen(['caelus.py', '-l', 'createPatch', '-overwrite'], shell=pltfrm)
run.wait()
run = None

# Executing setFields
print "Executing setFields"
run = subprocess.Popen(['caelus.py', '-l', 'setFields'], shell=pltfrm)
run.wait()
run = None

# Executing decomposePar -force
print "Executing decomposePar -force"
run = subprocess.Popen(['caelus.py', '-l', 'decomposePar', '-force'], shell=pltfrm)
run.wait()
run = None

# Executing vofLPTSolver
print "Executing vofLPTSolver"
run = subprocess.Popen(['caelus.py', '-l', 'vofLPTSolver', '-parallel'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('ACCM_bubbleCol3D.foam', 'a').close()

