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
print "**********************************"
print "Starting %s %.2f simulation" % (code, version)
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

# Executing setFields
print "Executing setFields"
run = subprocess.Popen(['caelus.py', '-l', 'setFields'], shell=pltfrm)
run.wait()
run = None

# Executing vofSolver
print "Executing vofSolver"
run = subprocess.Popen(['caelus.py', '-l', 'vofSolver'], shell=pltfrm)
run.wait()
run = None
