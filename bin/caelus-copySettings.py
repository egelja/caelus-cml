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
#     caelus-copySettings
#
# Description
#     Copy Caelus settings from one case to another, without copying
#     the mesh or results
#------------------------------------------------------------------------------

"""Usage: caelus copySettings [-h, -help] <src> <dst>

Copy Caelus settings from one case to another, without copying the mesh or
results.

Options
-------
<src>      Source case directory to copy the settings from
<dst>      Destination case directory to copy the settings to
-h, -help  Display this help message.

"""

import shutil
import sys
import os
import os.path
import re


# parse options
if sys.argv[1] == '-h' or sys.argv[1] == '-help':
   print(__doc__)
   sys.exit(0)

if len(sys.argv[1:]) != 2:
   sys.stderr.write('Error: exactly two arguments required\n')
   sys.stderr.write(__doc__+'\n')
   sys.exit(1)

srcDir = os.path.abspath(sys.argv[1])
dstDir = os.path.abspath(sys.argv[2])

for d in ('constant', 'system'):
   dd = os.path.join(srcDir, d)
   if not os.path.isdir(dd):
      sys.stderr.write('Error: no "%s" directory in "%s\n"'%(d, srcDir))
      sys.stderr.write('       This does not appear to be a Caelus case\n')
      sys.exit(1)

# TODO:
#  - verify that it works with multiple mesh regions
#  - special treatment for starting with negative crank angles
#  - or even better, parse startTime in controlDict if available

for parent, dirs, files in os.walk(srcDir):
   # remove processor, time (except 0) and polyMesh directories
   for d in dirs:
      if re.match(r'processor[0-9]+|[1-9].*|\d+\.\d+|polyMesh$', d):
         print('skipped', os.path.join(parent, d))
         dirs.remove(d)
   # remove log and queuing system output
   for f in files:
      if re.match(r'log.*|.*\.log|foam\.[eo][1-9]*', f):
         print('skipped', os.path.join(parent, f))
         continue
      # now copy the file
      subdir = os.path.relpath(parent, srcDir)
      s = os.path.join(parent, f)
      d = os.path.join(dstDir, subdir, f)
      print('%s\t->\t%s'%(s,d))
      shutil.copy2(s, d)

# ------------------- vim: set sw=3 sts=3 ft=python et: ------------ end-of-file
