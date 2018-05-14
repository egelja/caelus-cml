#!/usr/bin/python
# ---------------------------------------------------------------------------
# Caelus 7.04
# Web:   www.caelus-cml.com
# ---------------------------------------------------------------------------

# Importing the required modules for Python
import subprocess
import sys
import os

# Code name and version
code = 'sphinx'
version = 'one' 


# Starting up the meshing and solving

if sys.platform == 'win32':
   pltfrm = True
else:
   pltfrm = False

print "**********************************"
print "Generating Theory documentation" # (code, version)
print "**********************************"
bashCommand = "sphinx-build -b html source/Theory Guides/Theory"
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()

print "**********************************"
print "Generating Validation documentation" # (code, version)
print "**********************************"

bashCommand = "sphinx-build -b html source/Validation Guides/Validation"
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()

print "**********************************"
print "Generating Tutorials documentation" # (code, version)
print "**********************************"
bashCommand = "sphinx-build -b html source/Tutorial Guides/Tutorial"
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()

print ""
print "Documentation completed. View html version of documentation in Guides/."
print ""
