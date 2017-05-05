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

# Starting up the meshing and solving
print "**********************************"
print "Cleaning mixerVessel2D tutorial   "
print "**********************************"

# Cleaning up the case
os.system('caelus-cleanCase.py')
os.system('caelus-clearPolyMesh.py')

