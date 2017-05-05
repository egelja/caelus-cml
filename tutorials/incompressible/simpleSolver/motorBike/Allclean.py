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

# Starting up the meshing and solving
print "**********************************"
print "Cleaning motorBike tutorial       "
print "**********************************"

# Cleaning up the case
os.system('caelus-cleanCase.py')

# Copying the 0 directory
if os.path.exists('0'):
	shutil.rmtree('0')

