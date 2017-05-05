#!/usr/bin/python

# ---------------------------------------------------------------------------
# Caelus 5.10
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

# Executing BlockMesh utility
print "Executing blockMesh"
run = subprocess.Popen(['caelus.py', '-l', 'blockMesh'], shell=pltfrm)
run.wait()
run = None

# Executing decomposePar -force
print "Executing decomposePar -force"
run = subprocess.Popen(['caelus.py', '-l', 'decomposePar', '-force'], shell=pltfrm)
run.wait()
run = None

# Executing cpbnsPIMPLE
print "Executing cpbnsPIMPLE"
run = subprocess.Popen(['caelus.py', '-l', 'cpbnsPIMPLE', '-parallel'], shell=pltfrm)
run.wait()
run = None

# Executing reconstructPar -latestTime
print "Executing reconstructPar -latestTime"
run = subprocess.Popen(['caelus.py', '-l', 'reconstructPar', '-latestTime'], shell=pltfrm)
run.wait()
run = None
