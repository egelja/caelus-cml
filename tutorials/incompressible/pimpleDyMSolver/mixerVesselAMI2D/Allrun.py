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

# Code name and version
code = 'Caelus'
version = 5.04

# Starting up the meshing and solving
print "***************************************************"
print "Starting %s %.2f simulation of mixerVesselAMI2D" % (code, version)
print "***************************************************"

if sys.platform == 'win32':
   pltfrm = True
else:
   pltfrm = False

# Cleaning up the case
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Executing BlockMesh utility
print "Executing blockMesh"
run = subprocess.Popen(['caelus.py', '-l', 'blockMesh'], shell=pltfrm)
run.wait()
run = None

# Executing topoSet utility
print "Executing topoSet"
run = subprocess.Popen(['caelus.py', '-l', 'topoSet'], shell=pltfrm)
run.wait()
run = None

# Executing decomposePar -force
print "Executing decomposePar -force"
run = subprocess.Popen(['caelus.py', '-l', 'decomposePar', '-force'], shell=pltfrm)
run.wait()
run = None

# Executing pimpleDyMSolver in parallel
print "Executing pimpleDyMSolver in parallel"

run = subprocess.Popen(['caelus.py', '-l', 'pimpleDyMSolver', '-parallel'], shell=pltfrm)
run.wait()
run = None

# Executing reconstructPar
print "Executing reconstructPar"
run = subprocess.Popen(['caelus.py', '-l', 'reconstructPar'], shell=pltfrm)
run.wait()
run = None

