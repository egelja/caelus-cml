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
os.system('CaelusClearPolyMesh.py')

# Executing BlockMesh utility
print "Executing blockMesh"
logfile = open('blockMesh.log', 'w')
run = subprocess.Popen(['blockMesh'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing setFields
print "Executing setFields"
logfile = open('setFields.log', 'w')
run = subprocess.Popen(['setFields'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing buoyantSimpleSolver
print "Executing buoyantSimpleSolver"
logfile = open('buoyantSimpleSolver.log', 'w')
run = subprocess.Popen(['buoyantSimpleSolver'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

