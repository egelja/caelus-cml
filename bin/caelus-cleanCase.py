#!/usr/bin/python
#-------------------------------------------------------------------------------
# Copyright (C) 2008-2012 FreeFOAM
#                          Michael Wild <themiwi@users.sf.net>
#                          Gerber van der Graaf <gerber_graaf@users.sf.net>
# Copyright (C) 2015 Applied CCM
#-------------------------------------------------------------------------------
# License
#    This file is part of CAELUS.
#
#    CAELUS is free software: you can redistribute it and/or modify it
#    under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    CAELUS is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#    for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.
#
# Script
#     caelus-cleanCase.py
#
# Description
#    Remove solver generated files. Does not remove the polyMesh
#
#------------------------------------------------------------------------------

"""Usage: caelus-cleanCase [-h, -help]

Remove generated files and directories

Options
-------
-h, -help      Display this help message.

"""

import os
import sys

def rmtree(path, ignore_errors=False, onerror=None):
   """Recursively delete all files/directories under `path`
   (if `path` exists)"""
   import shutil
   import os
   path = os.path.abspath(path)
   if os.path.isdir(path):
      shutil.rmtree(path, ignore_errors, onerror)
   elif os.path.isfile(path) or os.path.islink(path):
      os.remove(path)

def clean_case(case='.'):
   """Clean a case by removing generated files and directories"""
   import glob
   import shutil
  
   case=os.path.abspath(case)
   patterns = ['[1-9]*', '-[1-9]*', 'log', 'log.*', 'log-*', 'log*.*',
         '*.log','processor*', 'probes*', 'forces*', 'VTK', '*.*~',
         'surfaceSampling', 'sets', 'postProcessing','*.foam','States',
         'postProcessedWaves','surfaceElevation*','waveGaugesNProbes']
   for nZeros in range(8):
       timeDir = '0.'+nZeros*'0'+'[1-9]*'
       for p in ['', '-']:
          patterns.append(os.path.join(case, p+timeDir))
   for p in patterns:
      for dd in glob.glob(os.path.join(case, p)):
         rmtree(dd)

# parse options
case = '.'
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-h' or a == '-help':
      print(__doc__)
      sys.exit(0)
   else:
      sys.stderr.write('Error: unknown argument "%s"\n'%a)
      sys.stderr.write(__doc__+'\n')
      sys.exit(1)

clean_case(case)

