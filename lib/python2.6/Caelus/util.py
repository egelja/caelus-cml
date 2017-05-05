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
#   Useful Python functionality
#
#-------------------------------------------------------------------------------

"""Classes and functions to manage Caelus cases."""

from Caelus.compat import *

import sys as _sys
import os as _os
import os.path as _op
import re as _re
import Caelus as _f

class NoPolyMeshDirectoryError(Exception):
   """Thrown if no polyMesh directory could be found"""
   def __init__(self):
      Exception.__init__(self)

   def __str__(self):
      return 'Failed to find a polyMesh directory'

class Tee:
   """A simple class which writes both to a file and standard output.

   It only implements the following functions from `file`:
      * `Tee.write`
      * `Tee.writelines`
      * `Tee.flush`
      * `Tee.fileno`

   """
   def __init__(self, fname, *args):
      """Initializes a new instance of `Tee`.

      Parameters
      ----------
      fname     : name of the file to write to
      mode      : file mode (as in `open()`)
      buffering : buffering option (as in `open()`)

      """
      self._file = open(fname, *args)
      self._stdout = _os.fdopen(_os.dup(_sys.stdout.fileno()), 'w')

   def write(self, string):
      """Write `string` to the file and standard output"""
      self._file.write(string)
      self._stdout.write(string)

   def writelines(self, strings):
      """Write `strings` to the file and standard output"""
      self._file.writelines(strings)
      self._stdout.writelines(strings)

   def flush(self):
      """Flush the streams"""
      self._file.flush()
      self._stdout.flush()

   def fileno(self):
      """Return the handle of the file stream"""
      return self._file.fileno()

def rm_rf(d):
   for path in (_os.path.join(d,f) for f in _os.listdir(d)):
      if _os.path.isdir(path):
         rm_rf(path)
      else:
         _os.unlink(path)
   _os.rmdir(d)

def clone_case(old_case, new_case, verbose=True):
   """Clone a Caelus case if the `new_case+'/system'` directory doesn't exist


   Parameters
   ----------
   old_case : The source case to be cloned.
   new_case : The destination case to be created.

   """
   if not _op.exists(_op.join(new_case, 'system')):
      if verbose: echo('Cloning case %s'%old_case)
      for d in ['0', 'system', 'constant', 'chemkin']:
         if _op.isdir(_op.join(old_case, d)):
            copytree(
                  _op.join(old_case, d),
                  _op.join(new_case, d),
                  symlinks=True)

def remove_case(case='.', verbose=True):
   """Removes a case directory"""
   echo('Removing case %s'%case)
   rmtree(case)

def clean_case(case='.', verbose=True):
   """Clean a case by removing generated files and directories"""
   import glob
   import shutil
   case=_op.abspath(case)
   if verbose: echo('Cleaning case', case)
   patterns = ['[1-9]*', '-[1-9]*', 'log', 'log.*', 'log-*', 'logSummary.*',
         '.fxLock', '*.xml', 'ParaView*', 'paraFoam*', '*.OpenFOAM',
         'processor*', 'probes*', 'forces*', 'system/machines', 'VTK']
   for nZeros in range(8):
       timeDir = '0.'+nZeros*'0'+'[1-9]*'
       for p in ['', '-']:
          patterns.append(_op.join(case, p+timeDir))
   for p in ('allOwner cell face meshModifiers owner neighbour point edge'+
         'cellLevel pointLevel refinementHistory surfaceIndex sets').split():
      patterns.append(_op.join(case, 'constant', 'polyMesh', p+'*'))
   for p in 'cellToRegion cellLevel pointLevel'.split():
      patterns.append(_op.join(case, 'constant', p+'*'))
   for p in patterns:
      for dd in glob.glob(_op.join(case, p)):
         rmtree(dd)

def clean_samples(case='.'):
   """Remove sample directories"""
   for d in 'sets samples sampleSurfaces'.split():
      rmtree(d)

def rmtree(path, ignore_errors=False, onerror=None):
   """Recursively delete all files/directories under `path`
   (if `path` exists)"""
   import shutil
   import os
   path = _op.abspath(path)
   if _op.isdir(path):
      shutil.rmtree(path, ignore_errors, onerror)
   elif _op.isfile(path) or _op.islink(path):
      os.remove(path)

def copytree(src, dst, symlinks=True):
   """Recursively copies `src` to `dst`. If `dst` exists,
   it is removed first."""
   import shutil
   src = _op.abspath(src)
   dst = _op.abspath(dst)
   rmtree(dst)
   parent = _op.dirname(dst)
   if not _op.isdir(parent):
      _os.makedirs(parent)
   if _op.isdir(src):
      shutil.copytree(src, dst, symlinks)
   else:
      shutil.copy2(src, dst)

def rename(src, dst):
   """Forcebly renames `src` to `dst`. `dst` is removed first if it exists."""
   import shutil
   src = _op.normpath(src)
   dst = _op.normpath(dst)
   if _op.exists(dst):
      rmtree(dst)
   shutil.move(src, dst)


def clear_polymesh(case=None, region=None):
   """Remove the contents of the constant/polyMesh directory as per the
   CML::polyMesh::removeFiles() method.

   Parameters
   ----------
   case   : Case directory from which to clear the mesh
   region : Mesh region to clear.

   """
   meshDir = 'polyMesh'
   if region:
      meshDir = _op.join(region, meshDir)
   tmp = _op.join('constant', meshDir)
   if case:
      meshDir = _op.join(case, tmp)
   else:
      if _op.isdir(tmp):
         meshDir = tmp
      elif _op.isdir(meshDir):
         # probably already in 'constant'
         pass
      elif _op.basename(_os.getcwd()) == 'polyMesh':
         # probably already in 'polyMesh'
         meshDir = _os.getcwd()

   if not _op.isdir(meshDir):
      raise NoPolyMeshDirectoryError()

   for f in """points faces owner neighbour cells boundary pointZones faceZones
   cellZones meshModifiers parallelData sets cellLevel pointLevel
   refinementHistory surfaceIndex cellToPatchFaceAddressing faceToPatchEdgeAddressing
   faceToPatchEdgeAddressing pointToPatchPointAddressing faceToPatchFaceAddressing""".split():
      f = _op.join(meshDir, f)
      if _op.isfile(f):
         _os.remove(f)

   for fgz in """points.gz faces.gz owner.gz neighbour.gz cells.gz boundary.gz pointZones.gz faceZones.gz
   cellZones.gz meshModifiers.gz parallelData.gz sets.gz cellLevel.gz pointLevel.gz
   refinementHistory.gz surfaceIndex.gz cellToPatchFaceAddressing.gz faceToPatchEdgeAddressing.gz
   faceToPatchEdgeAddressing.gz pointToPatchPointAddressing.gz faceToPatchFaceAddressing.gz""".split():
      fgz = _os.path.join(meshDir, fgz)
      if _os.path.isfile(fgz):
         _os.remove(fgz)

   for d in """sets""".split():
      d = _os.path.join(meshDir, d)
      if _os.path.isdir(d):
         rm_rf(d)

def make_short_path(path, maxlen=40):
   """Abbreviate `path` to a maximum length of `maxlen` characters."""
   path = _op.abspath(path)
   first = 2-maxlen
   if len(path) > maxlen:
      idx = path.find(_os.sep, first)
      if idx < 0:
         idx = first
      path = '...'+path[idx:]
   return path

class TerminalController:
   """
   A class that can be used to portably generate formatted output to
   a terminal.

   `TerminalController` defines a set of instance variables whose
   values are initialized to the control sequence necessary to
   perform a given action.  These can be simply included in normal
   output to the terminal:

       >>> term = TerminalController()
       >>> print 'This is '+term.GREEN+'green'+term.NORMAL

   Alternatively, the `render()` method can used, which replaces
   '${action}' with the string required to perform 'action':

       >>> term = TerminalController()
       >>> print term.render('This is ${GREEN}green${NORMAL}')

   If the terminal doesn't support a given action, then the value of
   the corresponding instance variable will be set to ''.  As a
   result, the above code will still work on terminals that do not
   support color, except that their output will not be colored.
   Also, this means that you can test whether the terminal supports a
   given action by simply testing the truth value of the
   corresponding instance variable:

       >>> term = TerminalController()
       >>> if term.CLEAR_SCREEN:
       ...     print 'This terminal supports clearning the screen.'

   Finally, if the width and height of the terminal are known, then
   they will be stored in the `COLS` and `LINES` attributes.

   Copied from http://code.activestate.com/recipes/475116-using-terminfo-for-portable-color-output-cursor-co
   """
   # Cursor movement:
   BOL = ''             #: Move the cursor to the beginning of the line
   UP = ''              #: Move the cursor up one line
   DOWN = ''            #: Move the cursor down one line
   LEFT = ''            #: Move the cursor left one char
   RIGHT = ''           #: Move the cursor right one char

   # Deletion:
   CLEAR_SCREEN = ''    #: Clear the screen and move to home position
   CLEAR_EOL = ''       #: Clear to the end of the line.
   CLEAR_BOL = ''       #: Clear to the beginning of the line.
   CLEAR_EOS = ''       #: Clear to the end of the screen

   # Output modes:
   BOLD = ''            #: Turn on bold mode
   BLINK = ''           #: Turn on blink mode
   DIM = ''             #: Turn on half-bright mode
   REVERSE = ''         #: Turn on reverse-video mode
   NORMAL = ''          #: Turn off all modes

   # Cursor display:
   HIDE_CURSOR = ''     #: Make the cursor invisible
   SHOW_CURSOR = ''     #: Make the cursor visible

   # Terminal size:
   COLS = None          #: Width of the terminal (None for unknown)
   LINES = None         #: Height of the terminal (None for unknown)

   # Foreground colors:
   BLACK = BLUE = GREEN = CYAN = RED = MAGENTA = YELLOW = WHITE = ''

   # Background colors:
   BG_BLACK = BG_BLUE = BG_GREEN = BG_CYAN = ''
   BG_RED = BG_MAGENTA = BG_YELLOW = BG_WHITE = ''

   _STRING_CAPABILITIES = """
   BOL=cr UP=cuu1 DOWN=cud1 LEFT=cub1 RIGHT=cuf1
   CLEAR_SCREEN=clear CLEAR_EOL=el CLEAR_BOL=el1 CLEAR_EOS=ed BOLD=bold
   BLINK=blink DIM=dim REVERSE=rev UNDERLINE=smul NORMAL=sgr0
   HIDE_CURSOR=civis SHOW_CURSOR=cnorm""".split()
   _COLORS = """BLACK BLUE GREEN CYAN RED MAGENTA YELLOW WHITE""".split()
   _ANSICOLORS = "BLACK RED GREEN YELLOW BLUE MAGENTA CYAN WHITE".split()

   def __init__(self, term_stream=_sys.stdout):
      """
      Create a `TerminalController` and initialize its attributes
      with appropriate values for the current terminal.
      `term_stream` is the stream that will be used for terminal
      output; if this stream is not a tty, then the terminal is
      assumed to be a dumb terminal (i.e., have no capabilities).
      """
      # Curses isn't available on all platforms
      try: import curses
      except: return

      # If the stream isn't a tty, then assume it has no capabilities.
      if hasattr(_sys.stdout, 'isatty'):
           if not term_stream.isatty(): return

      # Check the terminal type.  If we fail, then assume that the
      # terminal has no capabilities.
      try: curses.setupterm()
      except: return

      # Look up numeric capabilities.
      self.COLS = curses.tigetnum('cols')
      self.LINES = curses.tigetnum('lines')

      # Look up string capabilities.
      for capability in self._STRING_CAPABILITIES:
         (attrib, cap_name) = capability.split('=')
         setattr(self, attrib, self._tigetstr(cap_name) or '')

      # Colors
      set_fg = self._tigetstr('setf')
      if set_fg:
         for i,color in enumerate(self._COLORS):
            setattr(self, color,
                  self._tparm(set_fg, i) or '')
      set_fg_ansi = self._tigetstr('setaf')
      if set_fg_ansi:
         for i,color in enumerate(self._ANSICOLORS):
            setattr(self, color,
                  self._tparm(set_fg_ansi, i) or '')
      set_bg = self._tigetstr('setb')
      if set_bg:
         for i,color in enumerate(self._COLORS):
            setattr(self, 'BG_'+color,
                  self._tparm(set_bg, i) or '')
      set_bg_ansi = self._tigetstr('setab')
      if set_bg_ansi:
         for i,color in enumerate(self._ANSICOLORS):
            setattr(self, 'BG_'+color,
                  self._tparm(set_bg_ansi, i) or '')

   def _tigetstr(self, cap_name):
      # String capabilities can include "delays" of the form "$<2>".
      # For any modern terminal, we should be able to just ignore
      # these, so strip them out.
      import curses
      cap = curses.tigetstr(cap_name)
      if cap:
         cap = cap.decode()
      else:
         cap = ''
      return _re.sub(r'\$<\d+>[/*]?', '', cap)

   def _tparm(self, parm, i):
      import curses
      res = curses.tparm(parm, i)
      if res:
         res = res.decode()
      return res

   def render(self, template):
      """
      Replace each $-substitutions in the given template string with
      the corresponding terminal control string (if it's defined) or
      '' (if it's not).
      """
      return _re.sub(r'\$\$|\${\w+}', self._render_sub, template)

   def _render_sub(self, match):
      s = match.group()
      if s == '$$': return s
      else:
         return getattr(self, s[2:-1])

def cecho(*args, **kwargs):
   """print() replacement which does colored output if supported.

   The following sequences will be replaced in the `args`:
      * ${BOLD}       : Turn on bold mode
      * ${BLINK}      : Turn on blink mode
      * ${DIM}        : Turn on half-bright mode
      * ${REVERSE}    : Turn on reverse-video mode
      * ${NORMAL}     : Turn off all modes
      * ${<COLOR>}    : Set text color to <COLOR>
      * ${<BG_COLOR>} : Set background color to <BG_COLOR>

   The available colors are:
      * BLACK
      * BLUE
      * GREEN
      * CYAN
      * RED
      * MAGENTA
      * YELLOW
      * WHITE

    The valid background colors are:
      * BG_BLACK
      * BG_BLUE
      * BG_GREEN
      * BG_CYAN
      * BG_RED
      * BG_MAGENTA
      * BG_YELLOW
      * BG_WHITE

   """
   term = TerminalController()
   parts = list(args)
   for i, s in enumerate(parts):
      if type(s) == str:
         parts[i] = term.render(s)
   echo(*parts, **kwargs)

def cerror(*args, **kwargs):
   """Print a message, preceded with a bold, red '*** Error***' flag"""
   cecho("${RED}${BOLD}*** Error ***${NORMAL}", *args, **kwargs)

# ------------------------- vim: set sw=3 sts=3 et: --------------- end-of-file
