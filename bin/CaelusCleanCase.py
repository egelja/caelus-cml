#!/usr/bin/python
# Script
#     CaelusCleanCase
#
# Description
#    Remove solver generated files. Does not remove the polyMesh
#
#------------------------------------------------------------------------------

"""Usage: CaelusCleanCase [-h, -help]

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
         'surfaceSampling', 'sets']
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

