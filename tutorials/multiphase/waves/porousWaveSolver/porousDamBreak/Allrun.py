#!/usr/bin/python
# ---------------------------------------------------------------------------
# Caelus 7.04
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
print "Cleaning case"
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

# Copy of alpha1 field
shutil.copy2('0/alpha1.org', '0/alpha1')

# Copy of pressure field
shutil.copy2('0/p_rgh.org', '0/p_rgh')

# Copy of velocity field
shutil.copy2('0/U.org', '0/U')

# Executing BlockMesh utility
print "Executing blockMesh"
run = subprocess.Popen(['caelus.py', '-l', 'blockMesh'], shell=pltfrm)
run.wait()
run = None

# Compute the wave parameters
print "Executing setWaveParameters"
run = subprocess.Popen(['caelus.py', '-l', 'setWaveParameters'], shell=pltfrm)
run.wait()
run = None

# Set fields
print "Executing setFields"
run = subprocess.Popen(['caelus.py', '-l', 'setFields'], shell=pltfrm)
run.wait()
run = None

# Define the wave gauges/surface elevation probes
print "Executing waveGaugesNProbes"
run = subprocess.Popen(['caelus.py', '-l', 'waveGaugesNProbes'], shell=pltfrm)
run.wait()
run = None

# Executing porousWaveSolver
print "Executing porousWaveSolver"
run = subprocess.Popen(['caelus.py', '-l', 'porousWaveSolver'], shell=pltfrm)
run.wait()
run = None

# Executing postProcessWaves
print "Executing postProcessWaves"
run = subprocess.Popen(['caelus.py', '-l', 'postProcessWaves'], shell=pltfrm)
run.wait()
run = None

# Create stub file for paraview
open('porousDamBreak.foam', 'a').close()
