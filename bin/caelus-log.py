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
#     caelus-log
#
# Description
#     Extracts info from log file
#
# Bugs
#     -solution singularity not handled
#------------------------------------------------------------------------------

"""Usage: caelus log [options] <log>

Extracts xy files from Foam logs.

Options
-------
-case <case_dir> Case directory (defaults to $PWD)
-n               Produce single-column data files
-s               Operate silently
-l               Only list extracted variables
-h, -help        Print this help message
<log>            Log file from which to extract data. If <log> is not an
                 absolute path, it is relative to the specified case directory.

The default is to extract for all the 'Solved for' variables the initial
residual, the final residual and the number of iterations. On top of this a
(user editable) database of standard non-solved for variables is used to
extract data like Courant number, execution time.

The -l option shows all the possible variables but does not extract them.

The program writes a set of files, <case_dir>/logs/<var>_<subIter>, for every
<var> specified, for every occurrence inside a time step.

For variables that are 'Solved for' the initial residual name will be <var>,
the final residual will get name <var>FinalRes,

The files are a simple xy format with the first column Time (default) and the
second the extracted values. Option -n creates single column files with the
extracted data only.

The query database is a simple text format containing Python regular
expressions. The regular expression must capture the queried value in a group
with the values name (i.e. using (?P<name>...) syntax). Lines where the first
non-blank character is a # will be ignored. The database will either be
<case_dir>/caelusLog.db, $HOME/.Caelus/caelusLog.db or
$CAELUS_PROJECT_DIR/etc/caluesLog.db, whichever is found first.

Option -s suppresses the default information and only prints the extracted
variables.

"""


import os
import os.path
import sys
import re

from Caelus.compat import *
import Caelus

class PrintLog:
   def __init__(self, verbose):
      self.verbose = verbose
   def __call__(self, *args):
      if self.verbose:
         echo(*args)

class InvalidRegex(Exception):
   """Raised if a regex fails to compile"""
   def __init__(self, regex, i, msg):
      """Initialize with the error message `msg`, thefailed regex `regex` in
      line `i`"""
      Exception.__init__(self, regex, i, msg)

   def __str__(self):
      return 'Failed to compile regular expression "%s" in line %d:\n  %s'%self.args


def getSolvedForRegex(logf):
   """Extracts from the file object `logf` the solved-for variables and
   generates a list of regular expression objects to extract them or `None`."""
   p = logf.tell()
   logf.seek(0, 0)
   vars = set()
   for l in logf:
      m = re.search(r'Solving for\s+(?P<varname>\w+)', l)
      if m:
         vars.add(m.group('varname'))

   result = None
   if len(vars):
      result = []
      for v in vars:
         result.append(re.compile((''.join([
            'Solving for\s+%(var)s,\s+',
            'Initial residual = (?P<%(var)s>\S+),\s+',
            'Final residual = (?P<%(var)sFinalRes>\S+),\s+',
            'No Iterations (?P<%(var)sIters>\S+)']))%{'var': v}))
   logf.seek(p, 0)
   return result

def getDbRegex(logf, dbf):
   """Extracts from the file object `dbf` the contained regular expression
   strings and returns a list with regular expression objects that match any of
   the lines in the file object `logf` at least once."""
   pl = logf.tell()
   logf.seek(0, 0)
   pd = dbf.tell()
   dbf.seek(0, 0)

   # read db file
   allRegex = {}
   i = 0
   for l in dbf:
      i += 1
      # try to compile the line (discarding empty and comment lines)
      if not re.match(r'^\s*(#|$)', l):
         try:
            rc = re.compile(l[:-1])
         except re.error:
            e = sys.exc_info()[1]
            raise InvalidRegex(l[:-1], i, str(e))
         allRegex[i] = rc

   # try to match the regexes and tranfer the succesful ones into `result`
   result = []
   keys = list(allRegex.keys())
   for l in logf:
      for i in keys:
         r = allRegex[i]
         if r.search(l):
            result.append(r)
            keys.remove(i)
      if not len(keys):
         break
   dbf.seek(pd, 0)
   logf.seek(pl, 0)

   return result

def resetCounters(counters):
   """Reset the sub-iter counters"""
   for i in counters.keys():
      counters[i] = 0

#-----------------------------
# Main
#-----------------------------

# parse options
noTime = False
silent = False
listOnly = False
logName = None
caseDir = os.getcwd()
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-s':
      silent = True
      del args[0]
   elif a == '-n':
      noTime = True
      del args[0]
   elif a == '-l':
      listOnly = True
      del args[0]
   elif a == '-h' or a == '-help':
      echo(__doc__)
      sys.exit(0)
   elif a == '-case':
      if len(args) < 2:
         sys.stderr.write('Error: -case requires argument\n')
         sys.stderr.write(__doc__+'\n')
         sys.exit(1)
      caseDir = args[1]
      del args[:2]
   elif a[0] == '-':
      sys.stderr.write('Error: unknown option "%s"\n'%a)
      sys.stderr.write(__doc__+'\n')
      sys.exit(1)
   else:
      logName = a
      del args[0]

plog = PrintLog(not silent)

if not logName:
   sys.stderr.write('Error: No log file specified')
   sys.stderr.write(__doc__+'\n')
   sys.exit(1)

if not os.path.isabs(logName):
   logName = os.path.join(caseDir, logName)

if not os.path.isfile(logName):
   sys.stderr.write('Error: No such file "%s"\n'%logName)
   sys.exit(1)

# find caelusLog.db
dbName = None
for n in (
      caseDir,
      os.path.expanduser('~/.Caelus'),
      os.path.normpath(Caelus.CONFIG_DIR)
      ):
   n = os.path.join(os.path.normpath(n), 'caelus-log.db')
   if os.path.isfile(n):
      dbName = n
      break
if not dbName:
   sys.stderr.write('Error: Failed to find caelus-log.db\n')
   sys.exit(1)

# open the db and log file
logFile = open(logName, 'rt')
dbFile = open(dbName, 'rt')

# fetch all the regexes
regex = getSolvedForRegex(logFile)
regex.extend(getDbRegex(logFile, dbFile))
dbFile.close()

# get all the variable names, create data and counter container
vars = []
data = {}
counters = {}
for r in regex:
   for n in r.groupindex.keys():
      vars.append(n)
      data[n] = []
      counters[n] = 0
vars.sort()

# check uniqueness
if len(vars) != len(set(vars)):
   cnt = {}
   for v in vars:
      if not v in cnt:
         cnt[v] = 0
      cnt[v] += 1
   for v, n in cnt.items():
      if n > 1:
         sys.stderr.write(
               'Error: multiple regular expressions for variable "%s"\n'%v)
   sys.exit(1)

# if -l specified, list variables
if listOnly:
   echo('\n'.join(vars))
   sys.exit(0)

logsDir = os.path.join(caseDir, 'logs')

plog('Using:')
plog('  log      : %s'%os.path.basename(logName))
plog('  database : %s'%dbName)
plog('  files to : %s'%os.path.basename(logsDir))
plog('')

if not os.path.isdir(logsDir):
   if os.path.exists(logsDir):
      sys.stderr.write('Error: `%s` exists but is not a directory\n'%logsDir)
      sys.exit(1)
   os.mkdir(logsDir)

# loop over lines, extract data
splitRegex = re.compile(r'\s*Time\s*=\s*(?P<time>\S+)')
iteration = 0
resetCounters(counters)
time = []
for l in logFile:
   # check for splitting regex
   m = splitRegex.match(l)
   if m:
      time.append(m.group('time'))
      resetCounters(counters)
      iteration += 1
      continue
   for r in regex:
      # check for data regex
      m = r.search(l)
      if m:
         for n, v in m.groupdict().items():
            while len(data[n]) <= counters[n]:
               data[n].append([])
            data[n][counters[n]].append(v)
            counters[n] += 1
logFile.close()

# loop over data and write
tLen = max(map(len, time))
for n, v in data.items():
   for i in range(len(v)):
      f = open(os.path.join(logsDir, '%s_%d'%(n, i)), 'wt')
      for j in range(min(len(time), len(v[i]))):
         if not noTime:
            f.write(('%-'+str(tLen)+'s  ')%time[j])
         f.write('%s\n'%v[i][j])
      f.close()
plog('Generated XY files for:')
plog('\n'.join(vars))

# ------------------- vim: set sw=3 sts=3 ft=python et: ------------ end-of-file
