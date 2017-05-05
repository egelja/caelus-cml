#!/usr/bin/python

# ---------------------------------------------------------------------------
# Caelus 4.10
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
version = 4.10

# Starting up the meshing and solving
print "**********************************"
print "Starting %s %.2f simulation" % (code, version)
print "**********************************"

# Cleaning up the case
os.system('CaelusCleanCase.py')
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
logfile = open('decomposePar.log', 'w')
run = subprocess.Popen(['decomposePar', '-force'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing pisoSolver in parallel
print "Executing pisoSolver in parallel"

if sys.platform == 'win32':
	logfile = open('pisoSolver.log', 'w')
	run = subprocess.Popen(['mpiexec', '-localonly', '8', 'pisoSolver', '-parallel'], stderr=logfile, stdout=logfile)
	run.wait()
	logfile.close()
	run = None
else:
	logfile = open('pisoSolver.log', 'w')
	run = subprocess.Popen(['mpirun', '-np', '8', 'pisoSolver', '-parallel'], stderr=logfile, stdout=logfile)
	run.wait()
	logfile.close()
	run = None

# Executing reconstructPar -latestTime
print "Executing reconstructPar -latestTime"
logfile = open('reconstructPar.log', 'w')
run = subprocess.Popen(['reconstructPar', '-latestTime'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

