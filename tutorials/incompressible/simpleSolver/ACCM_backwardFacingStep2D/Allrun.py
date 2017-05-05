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

# Executing simpleSolver
print "Executing simpleSolver"
logfile = open('simpleSolver.log', 'w')
run = subprocess.Popen(['simpleSolver'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None


