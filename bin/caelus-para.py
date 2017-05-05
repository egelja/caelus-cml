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
#     caelus-para
#
# Description
#     Start ParaView with the OpenFOAM plugin
#
#------------------------------------------------------------------------------

"""Usage: caelus para [-case <dir>] [-touch] [-h, -help]

Visualize the case <dir> with ParaView.

Options
-------
-case  <dir>  Specify alternative case or processor directory
-touch        Only create the .foam file

"""

import os
import os.path
import sys
import glob
import subprocess

from Caelus.compat import *

# parse options
case = '.'
touchOnly = False
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-case':
      if len(args) < 2:
         sys.stderr.write('Error: The -case option requires an argument\n')
         sys.exit(1)
      case = args[1]
      del args[0:2]
   elif a == '-touch':
      touchOnly = True
      del args[0]
   else:
      sys.stderr.write('Error: unknown option/argument "%s"\n'%a)
      sys.stderr.write(__doc__+'\n')
      sys.exit(1)

case = os.path.abspath(case)

# get a sensible caseName
caseName = os.path.basename(case)
caseFile = "%s.foam"%caseName

if touchOnly:
   open(caseFile, 'at')
   echo('created "%s"'%os.path.relpath(caseFile, os.getcwd()))
   sys.exit(0)

# parent directory for normal or parallel results
parentDir = case
if caseName[:9] == 'processor':
   parentDir = os.path.dirname(case)

# check existence of essential files
for check in 'controlDict fvSchemes fvSolution'.split():
   f = os.path.join(parentDir, 'system', check)
   if not os.path.isfile(f):
      sys.stderr.write('Error: File does not exist "%s"\n'%f)
      sys.exit(1)

try:
   if os.path.exists(caseFile):
      doDelete = False
   else:
      # only create/remove caseFile if it didn't already exist
      doDelete = True
      open(caseFile, 'at')
      echo('created temporary "%s"'%caseFile)

   # Try to find ParaView:
   paraview = None
   if 'CAELUS_PARAVIEW_APP' in os.environ:
      # Using a environment variable the user can set...
      paraview=os.environ['CAELUS_PARAVIEW_APP']
   elif os.path.exists('/opt/ParaView-3.14.1/bin/paraview'):
      # Using the CMake configure value...
      paraview="/opt/ParaView-3.14.1/bin/paraview"

   if not paraview and os.uname()[0] == 'Darwin':
      # On Darwin it might also be an app-bundle
      pv_dirs = []
      if 'CAELUS_PARAVIEW_APP' in os.environ:
         pv_dirs.append(os.environ['CAELUS_PARAVIEW_APP'])
      pv_dirs.extend('/Applications ~/Applications'.split())
      for d in pv_dirs:
         d = os.path.expanduser(d)
         for n in reversed(glob.glob(os.path.join(d, '[Pp]ara[Vv]iew*.app'))):
            tmp = os.path.join(n, 'Contents', 'MacOS', 'paraview')
            if os.path.isfile(tmp):
               paraview = tmp
               break

   if not paraview:
      # Otherwise notify the user
      sys.stderr.write('Error: Could not find ParaView 3. Either compile it or point\n' +
                       'CAELUS_PARAVIEW_APP to the full path of the paraview application.\n')
      sys.exit(1)
   # We want to do nice exit when running paraview to give paraview opportunity
   # to clean up
   if 'FOAM_ABORT' in os.environ:
      del os.environ['FOAM_ABORT']
   sys.exit(subprocess.call([paraview, '--data=%s'%caseFile]))
finally:
   if doDelete:
      os.remove(caseFile)

# ------------------- vim: set sw=3 sts=3 ft=python et: ------------ end-of-file
