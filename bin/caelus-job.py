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
#     caelus-job
#
# Description
#     Run a caelus job in background and redirect the output to a log-file.
#
#------------------------------------------------------------------------------

"""Usage: caelus job [-s] [-p] [-log <logfile>] [-fg]
                          [-h, -help] <application> [OPTIONS]

Run a caelus job in background and redirect the output to a log-file.

Options
-------
-s             Also send output to the terminal
-p             Parallel run of processors
-log <logfile> Specify logfile name for output, defaults to 'log'
-fg            Run the job in the foreground (i.e. don't background it)
-h, -help      Display this help message
<application>  The Caelus application to run
[OPTIONS]      Arguments and options to be passed to the application

"""

import Caelus.util
import Caelus.run
import os
import sys
import subprocess

from Caelus.compat import *

# parse options
screen = False
parallel = False
logName = 'log'
foreground = False
application = None
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-s':
      screen = True
      del args[0]
   elif a == '-p':
      parallel = True
      del args[0]
   elif a == '-log':
      if len(args) < 2:
         sys.stderr.write('Error: The -log option requires an argument\n')
         sys.exit(1)
      logName = args[1]
      del args[0:2]
   elif a == '-fg':
      foreground = True
      del args[0]
   elif a == '-h' or a == '-help':
      print(__doc__)
      sys.exit(0)
   elif a[0] == '-':
      sys.stderr.write('Error: unknown option "%s"\n'%a)
      sys.stderr.write(__doc__+'\n')
      sys.exit(1)
   else:
      application = a
      del args[0]
      break

if not application:
   sys.stderr.write("Error: No application specified")
   sys.stderr.write(__doc__+'\n')
   sys.exit(1)

# now look for the -case option in the remaining arguments
case = '.'
for i, a in enumerate(args):
   if a == '-case':
      case = args[i+1]
      del args[i:i+2]
      break

# if we are to run in the background, fork (MUST BE FIRST!)
if not foreground:
   pid = os.fork()
   if pid:
      # this is the parent process
      sys.exit(0)

# this is the child process continuing here (if we forked)

# create the log file
if screen:
   logFile = Caelus.util.Tee(logName, 'wt')
else:
   logFile = open(logName, 'wt')

# create the runner instance
if parallel:
   runner = Caelus.run.ParallelRunner()
else:
   runner = Caelus.run.Runner()

# run the thing (needs a bit of hackery...)
runner.run(application, case=case, parallel=parallel, args=args,
      stdout=subprocess.PIPE, stderr=subprocess.STDOUT, background=True)

# retrieve the output and write it to the log file as long as the thing runs
while True:
   lines = runner.getProcess().stdout.readlines()
   if len(lines) and not isinstance(lines[0], str):
      lines = list(map(lambda l: l.decode(), lines))
   logFile.writelines(lines)
   returncode = runner.poll()
   if returncode != None:
      break

sys.exit(returncode)

# ------------------- vim: set sw=3 sts=3 ft=python et: ------------ end-of-file
