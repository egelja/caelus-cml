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

# Executing decomposePar -force
print "Executing decomposePar -force"
logfile = open('decomposePar.log', 'w')
run = subprocess.Popen(['decomposePar', '-force'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing simpleSolver in parallel
print "Executing simpleSolver in parallel"

if sys.platform == 'win32':
	logfile = open('simpleSolver.log', 'w')
	run = subprocess.Popen(['mpiexec', '-localonly', '8', 'simpleSolver', '-parallel'], stderr=logfile, stdout=logfile)
	run.wait()
	logfile.close()
	run = None
else:
	logfile = open('simpleSolver.log', 'w')
	run = subprocess.Popen(['mpirun', '-np', '8', 'simpleSolver', '-parallel'], stderr=logfile, stdout=logfile)
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





