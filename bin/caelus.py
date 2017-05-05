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
#     caelus
#
#------------------------------------------------------------------------------

"""Usage: caelus [options] <appName> [<appOptions>]

Runs a specific version of a Caelus executable with given arguments. Can also
be used for parallel runs.

Options
-------
-b, --basedir  Use a different base directory to search for applications
               (default is $CAELUS_PROJECT_DIR/platforms/$BUILD_OPTION).
-l, --log      Send application output to a log file (name: "application".log).
-P, --path     Also use the system PATH variable (searched last).
-p, --print    Instead of executing the command, print the command line that
               would be used. Useful for command substitution for e.g. mpirun if
               your system doesn't support the execve(2) (or similar) system
               call.
-h, --help     Display this help message.
-v, --version  Display the Caelus version and exit.
<appName>      Name of the Caelus application to run.
<appOptions>   Options and arguments to be passed to <appName>.

Set CAELUS_PATH to specify an alternative list of paths (colon-separated)
where to search for applications.

If the -P option is supplied, the system PATH variable will be appended to the
search path.

"""

import sys
import os.path as op

from Caelus.compat import *

import Caelus
import Caelus.run
import Caelus.path

# parse arguments (can't use optparse because of single-dash long options)
appName = None
useLogFile = False
useSysPath = False
baseDir = Caelus.EXEC_DIR
printOnly = False
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-b' or a == '--basedir':
      if len(args) < 2:
         sys.stderr.write('Error: The %s option requires an argument\n'%a)
         sys.exit(1)
      baseDir = args[1]
      del args[0:2]
   elif a == '-l' or a == '--log':
      useLogFile = True
      del args[0]
   elif a == '-P' or a == '--path':
      useSysPath = True
      del args[0]
   elif a == '-p' or a == '--print':
      printOnly = True
      del args[0]
   elif a == '-v' or a == '--version':
      print('Caelus version '+Caelus.VERSION_FULL)
      sys.exit(0)
   elif a == '-h' or a == '--help':
      print(__doc__)
      sys.exit(0)
   else:
      break

# get the application name and remove it from the argument list
if not len(args):
   sys.stderr.write('Error: No Caelus application specified\n')
   sys.exit(1)
app = args[0]
del args[0]

# find -case and -parallel in arguments
case = '.'
if '-case' in args:
   idx = args.index('-case')
   if idx > len(args) - 2:
      sys.stderr.write('Error: The -case option requires an argument\n')
      sys.exit(1)
   case = args[idx+1]
   del args[idx:idx+2]
parallel = False
if '-parallel' in args:
   parallel = True
   args.remove('-parallel')

# create the log file
if useLogFile:
   logName = app + '.log'
   logFile = open(logName, 'wt')
else:
   logFile = None

# construct search path
search_path = Caelus.path.create_app_search_path(
      basedir=baseDir, syspath=useSysPath)

# run the application
# create the runner instance
if parallel:
   runner = Caelus.run.ParallelRunner(search_path)
else:
   runner = Caelus.run.Runner(search_path)

#runner = Caelus.run.Runner(search_path)
status = 0
try:
   if printOnly:
      print(runner.command_str(app, case=case, parallel=parallel, args=args))
   else:
      status = runner.run(app, case=case, parallel=parallel, args=args,
                           stdout=logFile, stderr=logFile)
except Caelus.run.ApplicationNotFoundError:
   e = sys.exc_info()[1]
   sys.stderr.write(str(e)+'\n')
   status = 1

sys.exit(status)

# ------------------------- vim: set sw=3 sts=3 et: --------------- end-of-file
