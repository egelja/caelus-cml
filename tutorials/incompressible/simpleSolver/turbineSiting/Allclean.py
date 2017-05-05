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

# Starting up the meshing and solving
print "Cleaning tutorial: turbingSiting"

# Cleaning up the case
os.system('caelus-cleanCase.py')

# Copying the 0 directory
if os.path.exists('0'):
   shutil.rmtree('0')

