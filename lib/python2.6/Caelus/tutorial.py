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
#   Useful Python functionality to run Caelus tutorial cases
#
#-------------------------------------------------------------------------------

"""Classes and functions useful for running Caelus tutorial applications."""

# want to be future proof
from Caelus.compat import *

import os as _os
import os.path as _op
import Caelus as _Caelus
import Caelus.run as _CaeRun
import Caelus.util as _CaeUtil

compile_config = _Caelus.BUILD_TYPE

class ConfiguringFailedError(Exception):
   """Thrown if configuring a tutorial application failed."""
   def __init__(self, bindir, logfile):
      """Initialize the exception object.

      Parameters
      ----------
      bindir  : The binary directory.
      logfile : Name of the logfile which marks the case as run.

      """
      Exception.__init__(self, bindir, logfile)

   def __str__(self):
      """Return string representation of the error."""
      return ('*** ERROR *** Configuring application %s failed.\n' +
              'Refer to the logfile %s for more information.')%self.args

class CompilingFailedError(Exception):
   """Thrown if the compilation of a tutorial application failed."""
   def __init__(self, bindir, logfile):
      """Initialize the exception object.

      Parameters
      ----------
      bindir  : The binary directory where the compilation should have run.
      logfile : Name of the logfile which marks the case as run.

      """
      Exception.__init__(self, bindir, logfile)

   def __str__(self):
      """Return string representation of the error."""
      return ('*** ERROR *** Cleaning application %s failed.\n' +
              'Refer to the logfile %s for more information.')%self.args

class CleaningFailedError(Exception):
   """Thrown if the cleaning of a tutorial application failed."""
   def __init__(self, bindir):
      """Initialize the exception object.

      Parameters
      ----------
      app     : The application that was already run.
      logfile : Name of the logfile which marks the case as run.

      """
      Exception.__init__(self, bindir)

   def __str__(self):
      """Return string representation of the error."""
      return ('*** ERROR *** Cleaning application %s failed.'%self.args)

class CaseRunner(object):
   """A case runner.

   Attributes
   ----------
   name            : Name of the case.
   case_dir        : Directory containing the case.
   test_dir        : Directory to use in test runs.
   default_schemes : Use the default finite volume schemes in test runs.
                     Defaults to False.

   """
   def __init__(self, name, case_dir='.', test_dir=None, skip_test=False):
      """Initializes a new CaseRunner instance.

      Parameters
      ----------
      name         : Name of the case.
      case_dir     : Directory containing the case. Defaults to
                     `os.getcwd()`.
      test_dir     : Directory to create when cloning the case for the test
                     run. Defaults to `case_dir-test`.
      skip_test    : Default value to use when adding a new step.

      """
      self.name = name
      self.case_dir = _op.abspath(case_dir)
      if test_dir == None:
         test_dir = self.case_dir+'-test'
      self.test_dir = _op.abspath(test_dir)
      self.skip_test = skip_test
      self.default_schemes = False
      self._stepnames = []
      self._steps = {}

   def name(self):
      """Returns the name of this tutorial."""
      return self._name

   def steps(self):
      """Retrieves the name of all registered steps."""
      return self._stepnames

   def add_step(self, stepname, cmd, stamp_file=None, skip_test=None):
      """Registers a callable object to be run as a tutorial step.

      Parameters
      ----------
      stepname   : Name of the step.
      cmd        : Callable (function or object with __call__ function).
      stamp_file : File to use as a stamp. If absolute, relative path to
                   `self.case_dir` will be used, and then appended to either
                   `self.case_dir` or `self.test_dir`. Defaults to
                   `'log.'+stepname`.
      skip_test  : Skip this step in test mode. Defaults to `self.skip_test`.

      Note
      ----
      `cmd` must have the following signature:
         def command(case_dir, stamp_file, test_mode):
            # execute step
            # write stamp file
            return result
      where `case_dir` is the the absolute path to where the case is contained
      in. `stamp_file` is a `file` object the command should write its log to.
      The last line must either read `REPORT: SUCCESS` or `REPORT: FAILURE` (as
      e.g. created by `RunApp.__call__()`). Upon success, the function returns
      True, False otherwise.

      """
      assert stepname not in self._stepnames, (
            "Step "+stepname+" already exists")
      if stamp_file == None:
         stamp_file = 'log.'+stepname
      if _op.isabs(stamp_file):
         stamp_file = _op.relpath(stamp_file, self.case_dir)
      if skip_test == None:
         skip_test = self.skip_test
      self._stepnames.append(stepname)
      self._steps[stepname] = {
            'cmd'       : cmd,
            'stamp'     : stamp_file,
            'skip_test' : skip_test,
            }

   def add_app_step(self, stepname, app=None, parallel=False, args=[],
                    stamp_file=None, skip_test=None):
      """Convenience function to add a step running a Caelus application.

      Parameters
      ----------
      stepname    : Name of the step.
      app         : Name of the application to run. Defaults to `stepname`.
      parallel    : If True, run the application in parallel.
      args        : Additional arguments to be passed to the application.
      stamp_file  : File to use as a stamp. Relative to `self.case_dir`.
                    Defaults to `'log.'+app`.
      skip_test   : Skip this step in test mode.

      """
      if app == None:
         app = stepname
      if stamp_file == None:
         stamp_file = 'log.'+_op.basename(stepname)
      self.add_step(stepname, RunApp(app, parallel, args),
            stamp_file=stamp_file, skip_test=skip_test)

   def run(self, step=None, test_mode=False, verbose=False):
      """Run the case as a whole or a specific step.

      A step is only run if at least one of the following applies:
         * `test_mode` is True.
         * The stamp file of the test (usually the log file) does not exist.
         * The stamp file of the preceeding step is newer than the one for this
           step.

      Every step is assumed to depend on the preceeding step, and above
      criteria propagate up to the first step if required (except for
      `test_mode` which only applies to the specified step, or to all steps if
      `step` is None). This means that if you specify a step to be run and any
      of the preceeding steps has not been run or is out of date, that step and
      all the ones in between will also be run.

      Parameters
      ----------
      step      : Name of the step to be run. If `None`, try to run all steps.
      test_mode : Run in testing mode.
      verbose   : Echo the output to standard output.

      Returns
      -------
      True if the steps all ran successfully, False otherwise.

      """
      import sys
      assert len(self._stepnames) > 0, "No steps registered"

      if step == None:
         step = self._stepnames[-1]
      assert step in self._stepnames, "Step "+step+" not registered"

      if test_mode:
         self.clone_test()
         case_dir = self.test_dir
      else:
         case_dir = self.case_dir
      case_dir = _op.abspath(case_dir)

      echo('Running case '+case_dir)

      stat = True
      fmt = '    %%-%ds  ${HIDE_CURSOR}'%max(
            30, max(map(len, self._stepnames)))
      last = self._stepnames.index(step)
      try:
         for i, s in enumerate(self._stepnames):
            if not verbose:
               _CaeUtil.cecho(fmt%s, end='')
            sys.stdout.flush()
            stamp = _op.join(case_dir, self._steps[s]['stamp'])
            do_skip = False
            if test_mode and self._steps[s]['skip_test']:
               do_skip = True
            else:
               if i > last:
                  do_skip = True
               elif _op.isfile(stamp):
                  do_skip = True
                  if i > 0:
                     old_s = self._stepnames[i-1]
                     old_stamp = _op.join(
                           case_dir, self._steps[old_s]['stamp'])
                     if _op.getmtime(old_stamp) > _op.getmtime(stamp):
                        do_skip = False
            if do_skip:
               if not verbose:
                  _CaeUtil.cecho('${YELLOW}[ SKIP ]${NORMAL}')
               continue

            try:
               if verbose:
                  stamp_file = _CaeUtil.Tee(stamp, 'wt')
               else:
                  stamp_file = open(stamp, 'wt')
               stat = self._steps[s]['cmd'](case_dir, stamp_file, test_mode)
               if not verbose:
                  if stat:
                     _CaeUtil.cecho('${GREEN}[  OK  ]${NORMAL}')
                  else:
                     _CaeUtil.cecho('${RED}${BOLD}[FAILED]${NORMAL}')
            except BaseException:
               e = sys.exc_info()[1]
               if not verbose:
                  _CaeUtil.cecho('${RED}${BOLD}[FAILED]${NORMAL}')
               _CaeUtil.cerror("Step %s in case %s raised an exception:\n%s"%
                     (s, case_dir, str(e)), file=sys.stderr)
               stat = False
               raise
            if not stat:
               break
      finally:
         _CaeUtil.cecho('${SHOW_CURSOR}', end='')
      return stat

   def clean(self):
      """Cleans the case.

      This function is supposed to be overridden by tutorials requiring more
      cleaning up than `Caelus.util.clean_case` provides.

      Note that this implementation also removes `test_dir` if it exist
      and is not equal to the `case_dir`.

      """
      _CaeUtil.clean_case(self.case_dir)
      if self.test_dir != self.case_dir and _op.isdir(self.test_dir):
         _CaeUtil.remove_case(self.test_dir)

   def clone_test(self):
      """Creates a testing clone.

      This function is supposed to be overridden by tutorials requiring more
      cloning work than `Caelus.util.clone()` and `modify_for_test()`
      provide.

      """
      if _op.isdir(self.test_dir):
         _CaeUtil.remove_case(self.test_dir)
      _CaeUtil.clone_case(self.case_dir, self.test_dir)
      self.modify_for_test()

   def modify_for_test(self):
      """Modifies the case `test_dir` for use as a test case."""
      self.modify_global_controlDict_for_test(self.test_dir)
      self.modify_case_for_test(self.test_dir)

   def modify_global_controlDict_for_test(self, test_dir):
      """Copies the global etc directory and modifies if for testing."""
      ctrld = _op.join(test_dir, 'etc', 'controlDict')
      if not _op.isdir(_op.dirname(ctrld)):
         _os.makedirs(_op.dirname(ctrld))
      _CaeUtil.copytree(_op.join(_Caelus.CONFIG_DIR, 'controlDict'), ctrld)
      _CaeUtil.copytree(
            _op.join(_Caelus.CONFIG_DIR, 'cellModels'),
            _op.join(test_dir, 'etc', 'cellModels'))
      _CaeUtil.copytree(
            _op.join(_Caelus.CONFIG_DIR, 'thermoData'),
            _op.join(test_dir, 'etc', 'thermoData'))
      modify_file(ctrld, [
         (r'(fvSchemes\s*)([0-9]);', r'\g<1>1;'),
         (r'(solution\s*)([0-9]);', r'\g<1>1;') ])

   def modify_case_for_test(self, test_dir):
      "Modifies system/{controlDict,fvSchemes,snappyHexMeshDict} for testing."
      import re
      # modify system/controlDict
      for e in ['', '.1st']:
         ctrld = _op.join(test_dir, 'system', 'controlDict'+e)
         if _op.isfile(ctrld):
            modify_file(ctrld, [
               (r'(startFrom\s+)([a-zA-Z]+);', r'\g<1>latestTime;'),
               (r'(stopAt\s+)([a-zA-Z]*);', r'\g<1>nextWrite;'),
               (r'(writeControl\s+)([a-zA-Z]+);', r'\g<1>timeStep;'),
               (r'(writeInterval\s+)([0-9a-zA-Z.-]+);', r'\g<1>1;'),
               ], '.orig')
      # modify system/fvSchemes
      if self.default_schemes:
         fv_schemes = (
             'gradScheme',
             'divScheme',
             'laplacianScheme',
             'interpolationScheme',
             'snGradScheme',
             'fluxRequired',
             )
         for parent, dirs, files in _os.walk(_op.join(test_dir,'system')):
            if 'fvSchemes' in files:
               fvs = _op.join(parent, 'fvSchemes')
               lines = open(fvs, 'rt').readlines()
               kill_lines = []
               n_brace = 0
               search_start = False
               search_end = False
               for i, l in enumerate(lines):
                  m = re.search('|'.join(fv_schemes), l)
                  if n_brace > 0 or search_start or m:
                     kill_lines.append(i)
                     if m:
                        n_brace = 0
                        search_start = True
                        search_end = False
                  if search_start or search_end:
                     n_brace += l.count('{')
                     if n_brace > 0:
                        search_start = False
                        search_end = True
                     n_brace -= l.count('}')
                     if search_end and n_brace < 1:
                        search_end = False
               for i in reversed(kill_lines):
                  del lines[i]
               f = open(fvs, 'wt')
               f.writelines(lines)
               f.write("""
gradSchemes { default Gauss linear; }
divSchemes
{
    default Gauss linear;
    div(phi,fu_ft_h) Gauss multivariateSelection
    {
        fu upwind;
        ft upwind;
        h  upwind;
    };
    div(phi,ft_b_h_hu) Gauss multivariateSelection
    {
        fu upwind;
        ft upwind;
        bprog  upwind;
        h  upwind;
        hu upwind;
    };
}
laplacianSchemes { default Gauss linear corrected; }
interpolationSchemes { default linear; }
snGradSchemes { default corrected; }
fluxRequired { default yes; }
"""                  )
               f.close()

class ClonedCaseRunner(CaseRunner):
   """A runner for cases that need to be cloned from another case."""
   def __init__(self, name, parent_runner, case_dir='.',
         test_dir=None, skip_test=False):
      CaseRunner.__init__(self, name, case_dir=case_dir,
            test_dir=test_dir, skip_test=skip_test)
      self._parent_runner = parent_runner

   def clone_from_parent(self, case_dir):
      """Clones the case from its parent case."""
      try:
         parent_dir = self._parent_runner.case_dir
         if not _op.isdir(_op.join(case_dir, 'system')):
            _CaeUtil.clone_case(parent_dir, case_dir, verbose=False)
         return True
      except Exception:
         import sys
         e = sys.exc_info()[1]
         _CaeUtil.cerror("Failed to clone", parent_dir, ":", str(e),
               file=sys.stderr)
         return False

   def run(self, step=None, test_mode=False, verbose=False):
      """Refer to `CaseRunner.run`."""
      if not test_mode:
         self.clone_from_parent(self.case_dir)
      return CaseRunner.run(self, step=step, test_mode=test_mode,
                            verbose=verbose)

   def clean(self):
      """Removes the cloned case and, if it exists, the test-case."""
      _CaeUtil.remove_case(self.case_dir)
      if _op.isdir(self.test_dir):
         _CaeUtil.remove_case(self.test_dir)

   def clone_test(self):
      """Refer to `CaseRunner.clean`."""
      if _op.isdir(self.test_dir):
         _CaeUtil.remove_case(self.test_dir)
      self.clone_from_parent(self.test_dir)
      self.modify_for_test()

class TutorialRunner(object):
   """A tutorial case runner.

   Attributes
   ----------
   default_schemes : Use the default finite volume schemes in test runs.
                     Defaults to False.
   keep_going      : Do not abort after a failed case. Defaults to False.
   test_mode       : Run in testing mode. Defaults to False.
   verbose         : Echo the output to standard output. Defaults to False.

   """
   def __init__(self, default_schemes=False, keep_going=False, test_mode=False,
         verbose=False):
      """Initializes a new TutorialRunner instance."""
      self.default_schemes = default_schemes
      self.keep_going = keep_going
      self.test_mode = test_mode
      self.verbose = verbose
      self.did_run = False
      self._cases = []

   def cases(self):
      """Returns a tuple of all registered case names."""
      return tuple([c.name for c in self._cases])

   def steps(self):
      """Returns a tuple of all registered step names."""
      steps = []
      for c in self._cases:
         for s in c.steps():
            steps.append('.'.join((c.name,s)))
      return steps

   def add_case(self, case):
      """Adds a case."""
      assert case.name not in self.cases(), (
            "Trying to re-register case "+case.name)
      self._cases.append(case)

   def run(self, step=None):
      """Run the case as a whole or a specific step.

      A step is only run if at least one of the following applies:
         * `self.test_mode` is True.
         * The stamp file of the test (usually the log file) does not exist.
         * The stamp file of the preceeding step of the same case is newer than
           the one for this step.

      Every step is assumed to depend on the preceeding step of the same case,
      and above criteria propagate up to the first step if required (except for
      `self.test_mode` which only applies to the specified step, or to all
      steps if `step` is None). This means that if you specify a step to be run
      and any of the preceeding steps of the same case has not been run or is
      out of date, that step and all the ones in between will also be run.

      In test mode all steps belonging to the same case are run (up to the
      specified one), except in the default case, where all steps in all cases
      are run.

      The names of the steps are composed of the case name and the step name,
      separated by a dot. Only specifying the case name is equivalent to
      running all the steps belonging to that case.

      Parameters
      ----------
      step      : Name of the step to be run. If `None`, try to run all steps.

      Returns
      -------
      True if the steps all ran successfully, False otherwise.

      """
      case = None
      substep = None
      if step != None:
         case_and_step = step.split('.', 1)
         case = case_and_step[0]
         if len(case_and_step) > 1:
            substep = case_and_step[1]
      cases = self.cases()
      if case:
         cases = [case]
      stat = False
      for c in filter(lambda c: c.name in cases, self._cases):
         c.default_schemes = self.default_schemes
         stat = False
         try:
            stat = c.run(substep, self.test_mode, self.verbose)
         except:
            if not self.keep_going:
               raise
         if not stat and not self.keep_going:
            break
      return stat or self.keep_going

   def clean(self):
      """Clean all cases."""
      for c in self._cases:
         c.clean()

   def main(self):
      """Run as the main function in a script.
      <step>             Optionally, a step can be specifiedo which should be
                         run. By default all steps are run. Refer to run() for
                         the details.

      """
      import optparse
      import sys
      parser = optparse.OptionParser(usage='Usage: %prog [options] [step]')
      parser.add_option('-t', '--test', dest='testmode', action='store_true',
            default=False, help=('Run in testing mode. This causes the case ' +
               'to be cloned to a test directory.'))
      parser.add_option('-k', '--keep-going', dest='keep_going',
            action='store_true', default=False,
            help='Don\'t abort after a failed case.')
      parser.add_option('-d', '--default-schemes', dest='default_schemes',
            action='store_true', default=False,
            help='Run using a set of default fvSchemes.')
      parser.add_option('-l', '--list', dest='list', action='store_true',
            default=False, help='List the individual steps in this tutorial.')
      parser.add_option('-c', '--cases', dest='cases', action='store_true',
            default=False, help='Print the cases in this tutorial.')
      parser.add_option('--clean', dest='clean', action='store_true',
            default=False, help='Clean this tutorial case.')
      parser.add_option('-v', '--verbose', dest='verbose', action='store_true',
            default=False, help=('Echo the output to standard output'))

      (opts, args) = parser.parse_args()
      if opts.list and (opts.testmode or opts.default_schemes or
            opts.keep_going or opts.cases or opts.clean or opts.verbose or
            len(args)>0):
         parser.error(
            'Option --list may not be used with arguments and other options')
      if opts.cases and (opts.testmode or opts.default_schemes or
            opts.keep_going or opts.list or opts.clean or opts.verbose or
            len(args)>0):
         parser.error(
            'Option --cases may not be used with arguments and other options')
      if opts.clean and (opts.testmode or opts.default_schemes or
            opts.keep_going or opts.list or opts.cases or opts.verbose or
            len(args)>0):
         parser.error(
            'Option --clean may not be used with arguments and other options')
      if opts.default_schemes and not opts.testmode:
         parser.error(
            'Option --default-schemes can only be used with --test')
      if len(args) > 1:
         parser.error(
            'May not be called with more than one argument')

      if opts.list:
         echo('\n'.join(self.steps()))
         return 0
      if opts.cases:
         echo('\n'.join(self.cases()))
         return 0
      if opts.clean:
         self.clean()
         return 0

      self.default_schemes = opts.default_schemes
      self.keep_going = opts.keep_going
      self.test_mode = opts.testmode
      self.verbose = opts.verbose
      self.did_run = True
      step = None
      if len(args):
         step = args[0]
      return int(not self.run(step))

class RunApp(object):
   """Functor to run a Caelus application."""
   def __init__(self, app, parallel=False, args=[]):
      """Creates a new application runner.

      Parameters
      ----------
      app         : Name or full/relative path to the application to run.
      parallel    : If True, the '-parallel' flag will be passed to the
                    application.
      args        : Additional arguments and options to be passed to the
                    application.

      """
      self._app = app
      self._parallel = parallel
      self._args = args
      self._runner = _CaeRun.ParallelRunner()

   def __call__(self, case_dir, stamp_file, testmode):
      """Runs the Caelus application.

      The function redirects the output to `self.logfile`. Before running
      the application, it writes a short header to the log, listing the
      application name, the case directory and the command line used to
      invoke the application. At the end of the run it writes a `REPORT:
      SUCCESS` if the application returned 0, or `REPORT: FAILURE` for all
      other return values or if an exception was caught.

      Returns
      -------
      True on success, False otherwise.

      """
      import sys
      import subprocess
      returncode = 1
      stamp_file.write(''.join([80*'='+'\nAPPLICATION: ', self._app, '\n']))
      stamp_file.write(''.join(['CASE: ', case_dir, '\n']))
      stamp_file.write(''.join(['COMMAND LINE: ', self._runner.command_str(
         self._app, case=case_dir, parallel=self._parallel,
         args=self._args), '\n'+80*'='+'\n\n']))
      stamp_file.flush()
      try:
         # this is ridiculous, but python 2.4 doesn't support try-except-finally
         try:
            if testmode:
               _os.environ['CAELUS_CONFIG_DIR'] = _op.join(case_dir, 'etc')
            self._runner.run(self._app, case=case_dir,
                  parallel=self._parallel, stdout=subprocess.PIPE,
                  stderr=subprocess.STDOUT, args=self._args, background=True) == 0
            # retrieve the output and write it to the log file as long as the thing runs
            while True:
               lines = self._runner.getProcess().stdout.readlines()
               if len(lines) and not isinstance(lines[0], str):
                  lines = list(map(lambda l: l.decode(), lines))
               stamp_file.writelines(lines)
               stat = self._runner.poll()
               if stat != None:
                  break
            stat = stat == 0
         except KeyboardInterrupt:
            stamp_file.write('\n*** Error *** KeyboardInterrupt\n')
            stat = False
            raise
         except Exception:
            e = sys.exc_info()[1]
            stamp_file.write('\n*** Error *** '+str(e)+'\n')
            stat = False
            raise
      finally:
         # write final verdict to the log and clean up
         stamp_file.write('\n'+80*'='+'\nREPORT: ')
         if stat:
            stamp_file.write('SUCCESS\n')
         else:
            stamp_file.write('FAILURE\n')
      return stat

class CompileApp(object):
   """Functor to compile a Caelus tutorial application.

   Attributes
   ----------
   srcdir    : The top-level source directory.
   bindir    : The top-level binary directory.
   app       : Absolute path to the binary application.

   """
   def __init__(self, srcdir, bindir=None, name=None):
      """Initializes a new CompileApp instance.

      Parameters
      ----------
      srcdir : The top-level source directory.
      bindir : The top-level binary directory (i.e. where to build the
               application). If set to None, defaults to 'srcdir+"-build"'.
      name   : Name of the application. Defaults to `os.path.basename(srcdir)`.

      """
      self.srcdir = _op.abspath(srcdir)
      if bindir == None:
         bindir = self.srcdir+'-build'
      if name == None:
         name = _op.basename(self.srcdir)
      self.bindir = bindir
      self.app = _op.join(self.bindir, _Caelus.EXE_PREFIX+name+_Caelus.EXE_SUFFIX)

   def __call__(self, case_dir, stamp_file, testmode):
      """Compile a Caelus application.

      Configures and compiles a Caelus tutorial application located in the
      source directory `self.srcdir`.

      """
      import subprocess
      # get the current working directory
      pwd = _os.getcwd()
      # create the binary directory and cd to it
      if not _op.exists(self.bindir):
         _os.makedirs(self.bindir)
      elif not _op.isdir(self.bindir):
         raise BinDirCreationError(self.bindir,
               'The path exists, but is not a directory')
      returncode = 1
      msg = None
      _os.chdir(self.bindir)
      # configure the project
      cmd = [
            _Caelus.CMAKE_COMMAND,
            '-DCMAKE_C_COMPILER='+_Caelus.CMAKE_C_COMPILER,
            '-DCMAKE_CXX_COMPILER='+_Caelus.CMAKE_CXX_COMPILER,
            '-DCMAKE_BUILD_TYPE='+compile_config,
            '-G', _Caelus.CMAKE_GENERATOR,
            self.srcdir
            ]
      stamp_file.write(''.join(['CONFIGURING: ', ' '.join(cmd),
         '\n'+80*'='+'\n\n']))
      stamp_file.flush()
      try:
         process = subprocess.Popen(cmd, stdout=subprocess.PIPE,
               stderr=subprocess.STDOUT)
         while True:
            lines = process.stdout.readlines()
            if len(lines) and not isinstance(lines[0], str):
               lines = list(map(lambda l: l.decode(), lines))
            stamp_file.writelines(lines)
            returncode = process.poll()
            if returncode != None:
               break
         stamp_file.write('\n'+80*'='+'\nREPORT: ')
         if returncode:
            msg = 'Failed to configure the project "%s" in "%s".'%(
                  self.srcdir, self.bindir)
            stamp_file.write('FAILURE\n')
            return False
         else:
            stamp_file.write('SUCCESS\n')
            # kick off the build
            cmd = [_Caelus.CMAKE_COMMAND, '--build', '.',
                  '--config', compile_config]
            stamp_file.write(''.join(['COMPILING: ', ' '.join(cmd),
               '\n'+80*'='+'\n\n']))
            stamp_file.flush()
            returncode = subprocess.call(
                  cmd, stdout=stamp_file, stderr=stamp_file)
            stamp_file.write('\n'+80*'='+'\nREPORT: ')
            if returncode:
               msg = 'Failed to build the application "%s" in "%s".'%(
                     self.srcdir, self.bindir)
               stamp_file.write('FAILURE\n')
               return False
            else:
               stamp_file.write('SUCCESS\n')
      finally:
         _os.chdir(pwd)
      return True


def clean_application(bindir):
   """Cleans a Caelus application.

   Cleans a Caelus tutorial application located in the binary directory
   `bindir`.

   Parameters
   ----------
   bindir : The top-level binary directory (i.e. where the build directory is.

   """
   import subprocess
   bindir = _op.normpath(bindir)
   returncode = 0
   if _op.isdir(bindir):
      cmd = [_Caelus.CMAKE_COMMAND, '--build', bindir, '--target', 'clean']
      returncode = subprocess.call(cmd)
      if returncode:
         raise CleaningFailedError(bindir)
   return

def modify_file(fname, regSub, backup=None):
   """Modifies file `fname` by applying the regular expression/substitution
   pairs in the iterable `regSub`. If `backup` is a string and not None, a
   backup file with the given suffix will be created before the modification
   takes place."""
   import re
   fname = _op.abspath(fname)
   if backup and type(backup) == str:
      import shutil
      shutil.copy2(fname, fname+backup)
   lines=open(fname, 'rt').readlines()
   for i, l in enumerate(lines):
      for p, s in regSub:
         if re.search(p, l):
            lines[i] = re.sub(p, s, l)
   open(fname, 'wt').writelines(lines)
