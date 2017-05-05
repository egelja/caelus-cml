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
# Description
#   Useful Python functionality to run Caelus cases
#
#-------------------------------------------------------------------------------

"""Classes and functions useful for running Caelus applications."""

# want to be future proof
from Caelus.compat import *

import os as _os
import os.path as _op
import Caelus as _Caelus

class ApplicationNotFoundError(Exception):
   """Thrown by run_job if the requested application could not be found."""
   def __init__(self, appname):
      """Initializes the exception object.

      Parameters
      ----------
      appname : The name of the application

      """
      Exception.__init__(self, "ERROR: Application not found: %s"%appname)

class BinDirCreationError(Exception):
   """Thrown if the creation of the binary directory by compile_application failed."""
   def __init__(self, bindir, msg):
      """Initialize the exception object.

      Parameters
      ----------
      bindir : The directory that could not be created.
      msg    : A message describing the error condition in more detail.

      """
      Exception.__init__(self, bindir, msg)

   def __str__():
      """Return string representation of the error."""
      return 'ERROR: Failed to create "%s": %s'%self.args

class EntryNotFoundError(Exception):
   """Thrown if an entry couldn't be found in a config file."""
   def __init__(self, entry, filename):
      """Initialize the exception object.

      Parameters
      ----------
      entry   : The entry that could not be found.
      filename: Name of the config file.

      """
      Exception.__init__(self, entry, filename)

   def __str__(self):
      """Return string representation of the error."""
      return ('ERROR: Failed to find entry %s in %s.'%self.args)

class NotDecomposedError(Exception):
   """Thrown if case should be run in parallel but is not decomposed."""
   def __init__(self, case):
      """Initialize the exception object.

      Parameters
      ----------
      case : The case that isn't decomposed

      """
      Exception.__init__(self, case)

   def __str__(self):
      """Return string representation of the error."""
      return ('ERROR: Cannot run case %s in parallel, '+
            'because it is not decomposed.'%self.args)


class Runner:
   """A class to run Caelus applications."""

   def __init__(self, search_path=_Caelus.search_path):
      """Initializes the Runner object.

      Parameters
      ----------
      path : The search path for Caelus applications.

      """
      # set up path
      self._search_path = search_path
      # holds the subprocess.Popen object of a running job
      self._process = None
      # the exit code of the last process that ran. None if the process is still
      # running or if no process ran at all so far.
      self.returncode = None

   def _create_command(self, appname, case, parallel, args):
      """Assembles the command line (as a list, suitable for subprocess)"""
      from Caelus.path import locate_app
      # find executable
      app = locate_app(appname, self._search_path)
      if not app:
         raise ApplicationNotFoundError(appname)

      # assemble command line
      command = [app]
      if case:
         command.append('-case')
         command.append(_op.normpath(case))
      if parallel:
         command.append('-parallel')
      if type(args) == str:
         args = [args]
      if len(args):
         command.extend(args)
      return command

   def _check_parallel(self, parallel):
      """Checks a few (currently OpenMPI) characteristics to
      detect whether this is parallel run and issues a warning if the
      `parallel` flag does not match."""
      import sys
      if 'OMPI_COMM_WORLD_SIZE' in _os.environ:
         if not parallel:
            sys.stderr.write('Warning: Probably running in OpenMPI, '+
                  'but -parallel not specified\n')
      else:
         if parallel:
            sys.stderr.write('Warning: Probably not running in OpenMPI, '+
                  'but -parallel specified\n')


   def run(self, appname, background=False, case=None, parallel=False,
           stdin=None, stdout=None, stderr=None, args=[]):
      """Run a Caelus job.

      Parameters
      ----------
      appname    : Name or full/relative path to the application to run.
      background : Run the application in the background.
      case       : Path to the case to run. Defaults to the current directory.
      parallel   : If True, the '-parallel' flag will be passed to the
                   application.
      stdin, stdout and stderr:
                   The program's standard input, output and error streams,
                   respectively. Valid values are a file descriptor, a
                   file object or None (in which case the system streams
                   will be used).
      args       : Iterable or string of additional arguments and options to be
                   passed to the application.

      Returns
      -------
      result : The exit code of the application if 'background' is False,
               None otherwise.

      Throws
      ------
      ApplicationNotFoundError : If the application 'appname' cannot be found.

      """
      import subprocess
      import sys
      #self._check_parallel(parallel)
      command = self._create_command(appname, case=case,
                   parallel=parallel, args=args)
      if stdin == None:
         stdin = sys.stdin
      if stdout == None:
         stdout = sys.stdout
      # make sure that the global controlDict is found
      if 'CAELUS_CONFIG_DIR' not in _os.environ:
         _os.environ['CAELUS_CONFIG_DIR'] = _Caelus.config_dir
      # run the command
      self.returncode = None
      self._process = subprocess.Popen(command, stdin=stdin, stdout=stdout,
                                       stderr=stderr)
      # should we run in the background?
      if background:
         return None
      # nope, so we wait, delete the _process object and return the exit code
      self._process.wait()
      self.returncode = self._process.returncode
      self._process = None
      return self.returncode

   def poll(self):
      """Check if child process has terminated.
      Set and return returncode attribute. If the process terminated, the
      object is freed.

      Returns
      -------
      returncode : The exit code of the process or None if the process is still
                   running or no process has ever been run by this object.

      """
      if self._process:
         self.returncode = self._process.poll()
      else:
         self.returncode = None
      return self.returncode

   def getProcess(self):
      """Access the Popen object."""
      return self._process

   def command_str(self, appname, case=None, parallel=False, args=[]):
      """Return the command line to run a Caelus job.

      Parameters
      ----------
      appname    : Name or full/relative path to the application to run.
      case       : Path to the case to run. Defaults to the current directory.
      parallel   : If True, the '-parallel' flag will be passed to the
                   application.
      args       : Iterable or string of additional arguments and options to be
                   passed to the application.

      Returns
      -------
      result : A string with the command line.

      Throws
      ------
      ApplicationNotFoundError : If the application 'appname' cannot be found.

      """
      command = self._create_command(appname, case=case,
                   parallel=parallel, args=args)
      return ' '.join(command)

class ParallelRunner(Runner):
   """Class to run Caelus cases in parallel."""

   # override the _create_command function
   def _create_command(self, appname, case, parallel, args):
      """Assembles the command line (as a list, suitable for subprocess)"""
      import re
      import glob
      import sys
      # if parallel desired, check whether decomposed:
      is_decomposed =_op.isdir(_op.join(case, 'processor0'))
      cmd = None
      if parallel and is_decomposed:
         # count number of processors
         nproc = len(glob.glob(_op.join(case,'processor*')))
         # create command from the template in global controlDict
         template = None
         controlDict = _op.join(_Caelus.config_dir, 'controlDict')
         for l in open(controlDict, 'rt'):
            if _Caelus.WHICH_OS == 'windows':
               m = re.match(r'\s*parRunWinTemplate\s+["\'](.*)["\']\s*;\s*$', l)
            else:
               m = re.match(r'\s*parRunTemplate\s+["\'](.*)["\']\s*;\s*$', l)
            if m:
               template = m.group(1)
         if not template:
            raise EntryNotFoundError('parRunTemplate', controlDict)

         cmd = (template%{
               'NPROCS': nproc,
               'PAROPTS': '',
               'APPLICATION': ' '.join(
                  Runner._create_command(self, appname, case, False, [])),
               'ARGS': ' '.join(args),
               }).split()
      elif parallel and not is_decomposed:
         raise NotDecomposedError(case)
      if not cmd:
         cmd = Runner._create_command(self, appname, case, False, args)
      return cmd

   def _check_parallel(self, parallel):
      """Do nothing."""
      pass
