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
print "**********************************"
print "Starting %s %s simulation" % (code, version)
print "**********************************"

if sys.platform == 'win32':
   pltfrm = True
else:
   pltfrm = False

# Cleaning up the case
os.system('caelus-cleanCase.py')
if os.path.exists('constant/polyMesh'):
   shutil.rmtree('constant/polyMesh')

# Copying the polyMesh from motorBikeSS directory
print "Copying the polyMesh from motorBikeSS directory"
shutil.copytree('../motorBikeSS/constant/polyMesh', 'constant/polyMesh')

# Copying the solution from motorBikeSS at 500 iteration
print "Copying the solution from motorBikeSS at 500 iteration"
source =os.listdir("../motorBikeSS/500/")
destination ="0/"
for files in source:
  if files.endswith(".gz"):
    shutil.copy(os.path.join('../motorBikeSS/500/',files), destination)

# Executing decomposePar -force
print "Executing decomposePar -force"
run = subprocess.Popen(['caelus.py', '-l', 'decomposePar', '-force'], shell=pltfrm)
run.wait()
run = None

# Executing pisoSolver in parallel
print "Executing pisoSolver in parallel"

run = subprocess.Popen(['caelus.py', '-l', 'pisoSolver', '-parallel'], shell=pltfrm)
run.wait()
run = None

# Executing reconstructPar -latestTime
print "Executing reconstructPar -latestTime"
run = subprocess.Popen(['caelus.py', '-l', 'reconstructPar', '-latestTime'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('motorBikeLES.foam', 'a').close()

