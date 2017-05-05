#!/usr/bin/python

# ---------------------------------------------------------------------------
# Caelus 5.10
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
print "Cleaning ACCM_flatPlate2D         "
print "**********************************"

# Cleaning up the case
os.system('caelus-cleanCase.py')

