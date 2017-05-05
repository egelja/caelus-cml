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

# Copying the 0 directory
if os.path.exists('0'):
	shutil.rmtree('0')

shutil.copytree('0.org', '0')

# Executing potentialSolver
print "Executing potentialSolver"
run = subprocess.Popen(['caelus.py', '-l', 'potentialSolver', '-noFunctionObjects', '-writep'], shell=pltfrm)
run.wait()
run = None

# Executing decomposePar -force
print "Executing decomposePar -force"
run = subprocess.Popen(['caelus.py', '-l', 'decomposePar', '-force'], shell=pltfrm)
run.wait()
run = None

# Executing simpleSolver in parallel
print "Executing simpleSolver in parallel"

run = subprocess.Popen(['caelus.py', '-l', 'simpleSolver', '-parallel'], shell=pltfrm)
run.wait()
run = None

# Executing reconstructPar -latestTime
print "Executing reconstructPar -latestTime"
run = subprocess.Popen(['caelus.py', '-l', 'reconstructPar', '-latestTime'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('motorBike.foam', 'a').close()

