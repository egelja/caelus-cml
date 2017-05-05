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

# Copying the 0 directory
if os.path.exists('0'):
	shutil.rmtree('0')

shutil.copytree('0.org', '0')

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

# Executing createBaffles
print "Executing createBaffles"
run = subprocess.Popen(['caelus.py', '-l', "createBaffles_old", "baffleFaces", "(baffle1Wall_0 baffle1Wall_1)", "-overwrite"], shell=pltfrm)
run.wait()
run = None

# Executing extrudeToRegionMesh
print "Executing extrudeToRegionMesh"
run = subprocess.Popen(['caelus.py', '-l', 'extrudeToRegionMesh', '-overwrite'], shell=pltfrm)
run.wait()
run = None

# Executing changeDictionary to setup the BCs
print "Executing changeDictionary to setup the BCs"
run = subprocess.Popen(['caelus.py', '-l', 'changeDictionary', '-dict', 'system/changeDictionaryDict.baffle'], shell=pltfrm)
run.wait()
run = None

# Executing changeDictionary to set BCs for the baffle region
print "Executing changeDictionary to set BCs for the baffle region"
run = subprocess.Popen(['caelus.py', '-l', 'changeDictionary', '-dict', 'system/changeDictionaryDict.baffleRegion', '-literalRE'], shell=pltfrm)
run.wait()
run = None

# Executing changeDictionary to set proper values at the region
print "Executing changeDictionary to set proper values at the region"
run = subprocess.Popen(['caelus.py', '-l', 'changeDictionary', '-region', 'baffleRegion', '-literalRE'], shell=pltfrm)
run.wait()
run = None

# Executing buoyantSimpleSolver
print "Executing buoyantSimpleSolver"
run = subprocess.Popen(['caelus.py', '-l', 'buoyantSimpleSolver'], shell=pltfrm)
run.wait()
run = None

