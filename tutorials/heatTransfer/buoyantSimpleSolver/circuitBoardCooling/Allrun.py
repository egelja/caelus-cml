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

# Copying the 0 directory
if os.path.exists('0'):
	shutil.rmtree('0')

shutil.copytree('0.org', '0')

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

# Executing topoSet utility
print "Executing topoSet"
logfile = open('topoSet.log', 'w')
run = subprocess.Popen(['topoSet'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing createBaffles
print "Executing createBaffles"
logfile = open('createBaffles.log', 'w')
run = subprocess.Popen(["createBaffles", "baffleFaces", "(baffle1Wall_0 baffle1Wall_1)", "-overwrite"], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing extrudeToRegionMesh
print "Executing extrudeToRegionMesh"
logfile = open('extrudeToRegionMesh.log', 'w')
run = subprocess.Popen(['extrudeToRegionMesh', '-overwrite'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing changeDictionary to setup the BCs
print "Executing changeDictionary to setup the BCs"
logfile = open('changeDictionary_BC.log', 'w')
run = subprocess.Popen(['changeDictionary', '-dict', 'system/changeDictionaryDict.baffle'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing changeDictionary to set BCs for the baffle region
print "Executing changeDictionary to set BCs for the baffle region"
logfile = open('changeDictionary_baffle.log', 'w')
run = subprocess.Popen(['changeDictionary', '-dict', 'system/changeDictionaryDict.baffleRegion', '-literalRE'], stderr=logfile, stdout=logfile)
run.wait()
logfile.close()
run = None

# Executing changeDictionary to set proper values at the region
print "Executing changeDictionary to set proper values at the region"
logfile = open('changeDictionary_region.log', 'w')
run = subprocess.Popen(['changeDictionary', '-region', 'baffleRegion', '-literalRE'], stderr=logfile, stdout=logfile)
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

