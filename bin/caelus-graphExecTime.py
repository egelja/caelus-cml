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
#     caelus-graphExecTime
#
# Description
#     This utility computes the time used per iteration in seconds and writes
#     it to the file 'executionTime.dat' in TecPlot format.
#------------------------------------------------------------------------------

"""Usage: caelus graphExecTime [-h, -help] <logFile>

This utility computes the time used per iteration in seconds and writes it to
the file 'executionTime.dat' in TecPlot format.

Options
-------
<logFile> The log file from which to extract the data.
-h, -help Display this help message.

"""

import sys
import os.path
import re

from Caelus.compat import *

if len(sys.argv[1:]) != 1:
   sys.stderr.write('Error: Require a log file as argument\n')
   sys.stderr.write(__doc__+'\n')
   sys.exit(1)

if sys.argv[1] == '-h' or sys.argv[1] == '-help':
   print(__doc__)
   sys.exit(0)

logFile = sys.argv[1]
if not os.path.isfile(logFile):
   sys.stderr.write('Error: no such file')
   sys.exit(1)

# parse the execution times from the log file and compute differences
regex = re.compile(r'ExecutionTime\s+=\s+(?P<time>\S+)')
tprev = 0
dt = []
for l in open(logFile, 'rt'):
   m = regex.match(l)
   if m:
      t = float(m.group('time'))
      dt.append(t-tprev)
      tprev = t

# write file header
outFile = open('ExecutionTime.dat', 'wt')
outFile.write("""Solver Performance
iterations
time/iteration
0 0
0 0
1
""")

# write data
if len(dt) > 0:
   outFile.write("""time
0
%d
"""%len(dt))

   for i, v in enumerate(dt):
      outFile.write('%d  %g\n'%(i+1, v))

outFile.close()

# ------------------- vim: set sw=3 sts=3 ft=python et: ------------ end-of-file
