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
#     caelus-clearPolyMesh
#
# Description
#    Remove the contents of the constant/polyMesh directory
#    as per the CML::polyMesh::removeFiles() method.
#
#------------------------------------------------------------------------------

"""Usage: caelus clearPolyMesh [-h, -help] [-case <dir>] [-region <name>]

Remove the contents of the constant/polyMesh directory as per the
CML::polyMesh::removeFiles() method.

Options
-------
-case <dir>    The case directory from which to clear the polyMesh
-region <name> The mesh region to clear
-h, -help      Display this help message.

"""

import sys

from Caelus.compat import *
import Caelus.util

# parse options
case = None
region = None
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-case':
      if len(args) < 2:
         sys.stderr.write('Error: The -case option requires an argument\n')
         sys.exit(1)
      case = args[1]
      del args[0:2]
   elif a == '-region':
      if len(args) < 2:
         sys.stderr.write('Error: The -region option requires an argument\n')
         sys.exit(1)
      region = args[1]
      del args[0:2]
   elif a == '-h' or a == '-help':
      print(__doc__)
      sys.exit(0)
   else:
      sys.stderr.write('Error: unknown argument "%s"\n'%a)
      sys.stderr.write(__doc__+'\n')
      sys.exit(1)

try:
   Caelus.util.clear_polymesh(case, region)
except Caelus.util.NoPolyMeshDirectoryError:
   e = sys.exc_info()[1]
   sys.stderr.write('Error: '+str(e)+'\n')
   sys.exit(1)

# ------------------- vim: set sw=3 sts=3 ft=python et: ------------ end-of-file
