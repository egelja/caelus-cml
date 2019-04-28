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
#     caelus-solverSweeps
#
# Description
#     Extracts statistics from a log file
#
#------------------------------------------------------------------------------

"""Usage: caelus solverSweeps [-h, -help] [<log>]

This utility script extracts solver statistics from a log file and prints them
to the screen. The extracted quantities are:
- execution time of the first iteration
- overall execution time
- the number of iterations
- the first iteration number
- the last iteration number
- the number of solver sweeps for the velocity and pressure fields

Options
-------
-h, -help  Print this help message
<log>      Analyze this log file. If this is -, read log from standard input.
           If not specified, the name of the file (NOT the data) is read from
           standard input.

"""

import os
import os.path
import sys
import re

logFile = None
logName = None
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-h' or a == '-help':
      print(__doc__)
      sys.exit(0)
   elif a == '-':
      logName = 'stdin'
      logFile = sys.stdin
      break
   elif a[0] == '-':
      sys.stderr.write('Error: unknown option "%s"\n'%a)
      sys.stderr.write(__doc__+'\n')
      sys.exit(1)
   else:
      logName = a
      break

if not logFile:
   if not logName:
      sys.stderr.write('Name of log file [log] : ')
      logName = sys.stdin.readline().strip()
   if os.path.isfile(logName):
      logFile = open(logName, 'rt')
   else:
      sys.stderr.write('Error: No such file "%s"\n'%logName)
      sys.exit(1)

# Main
#~~~~~~

program = None
runTime = []
time = []
pIter = 0
UIter = 0

progRegex = re.compile(r'^Exec\s+: (?P<program>\S+)')
execTimeRegex = re.compile(r'^ExecutionTime\s*=.*')
timeRegex = re.compile(r'^Time\s*=.*')
pRegex = re.compile(r'.*Solving for p,.*\s+(?P<niter>\d+)$')
URegex = re.compile(r'.*Solving for U.,.*\s+(?P<niter>\d+)$')
for l in logFile:
   l = l.strip()
   if not program:
      m = progRegex.match(l)
      if m:
         program = m.group('program')
         continue
   if execTimeRegex.match(l):
      runTime.append(l)
      continue
   if timeRegex.match(l):
      time.append(l)
      continue
   m = pRegex.match(l)
   if m:
      pIter += int(m.group('niter'))
      continue
   m = URegex.match(l)
   if m:
      UIter += int(m.group('niter'))
      continue

print('\nProgram: %s'%program)

print('\nRuntime:')
print('  1st iter  : %s'%runTime[0])
print('  overall   : %s'%runTime[-1])

print('\nSimulation:')
print('  steps: %d'%len(time))
print('  from : %s'%time[0])
print('  to   : %s'%time[-1])

print('\nSolver sweeps:')
print('  p           : %d'%pIter)
print('  U(U0,U1,U2) : %d'%UIter)

# ------------------- vim: set sw=3 sts=3 ft=python et: ------------ end-of-file
