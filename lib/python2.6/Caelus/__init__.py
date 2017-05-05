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
#   Useful Python functionality for the Caelus scripts.
#
#-------------------------------------------------------------------------------

"""A package for scripting Caelus with Python.

Modules
-------
Caelus          : Basic information (paths, version etc.)
Caelus.path     : Search path handling (mostly used internally)
Caelus.run    : Classes and functions used for running and compiling.

Classes
-------
VersionError     : Exception class which is raised if the Python version is
                   smaller than 2.6.
NoConfigDirError : Exception class which is raised if no global configuration
                   directory could be located.

Variables in `Caelus`
-----------------------
VERSION_FULL       : String containing the full version ('major.minor.patch')
EXEC_DIR           : Base directory where all Caelus applications have been
                     installed into.
CONFIG_DIR         : Name of the directory where the global configuration files
                     are installed.
DATA_DIR           : Directory where architecture-independent data files are
                     installed.
PY_SCRIPT_SUFFIX   : Suffix used for executable Python scripts ('.py' on Windows,
                     the empty string on Unix-like platforms).
EXE_PREFIX         : Prefix used in front of @PROJECT_NAME@ executable names.
EXE_SUFFIX         : Extension used for binary executable files ('.exe' on
                     Windows, the empty string on Unix-like platforms).
CMAKE_COMMAND      : Path to the 'cmake' executable.
CMAKE_GENERATOR    : The generator used to compile @PROJECT_NAME@.
CMAKE_C_COMPILER   : The C compiler used to compile @PROJECT_NAME@.
CMAKE_CXX_COMPILER : The C++ compiler used to compile @PROJECT_NAME@.
config_dir         : Configuration directory containing the global
                     `controlDict` file. Set to None if it can't be found.
search_path        : Search path on which to find @PROJECT_NAME@ applications.
                     Refer to the documentation of
                     Caelus.path.create_app_search_path on how it is
                     initialized.

"""

import sys as _sys
import os as _os
import os.path as _op

from Caelus.compat import *
from Caelus.setup import *

class VersionError(Exception):
   """Raised when the Python version is smaller than required."""
   def __init__(self, *args):
      Exception.__init__(self, *args)

class NoConfigDirError(Exception):
   """Raised when no global configuration directory could be found."""
   def __init__(self, *args):
      Exception.__init__(self, *args)

# Ensure that we have Python2.0 or newer
if _sys.hexversion < 0x020000f0:
   raise VersionError(PROJECT + ' requires at least Python version 2.0 to run')
elif _sys.hexversion < 0x020400f0:
   # warn if < 2.4 (untested)
   echo('WARNING: Only Python versions >= 2.4 are tested with @PROJECT_NAME@', file=_sys.stderr)

# Some universal variables
VERSION_FULL = PROJECT_VER

EXEC_DIR = BIN_PLATFORM_INSTALL
CONFIG_DIR = _op.join(CAELUS_PROJECT_DIR,'etc')
DATA_DIR = CAELUS_TUTORIALS

PY_SCRIPT_SUFFIX = ''
EXE_PREFIX = ''

if os.name == 'nt':
  EXE_SUFFIX = '.exe'
else:
  EXE_SUFFIX = ''

#BUILD_TYPE = _os.environ['BUILD_TYPE']

# configuration search path
_config_path = [
      _op.abspath(_op.expanduser('~/Caelus/5.04')),
      _op.abspath(_op.expanduser('~/.Caelus')),
      CONFIG_DIR
      ]
if 'CAELUS_CONFIG_DIR' in _os.environ:
   _config_path.insert(0, _os.environ['CAELUS_CONFIG_DIR'])

# configuration directory
config_dir = None
for d in _config_path:
   if (_op.isfile(_op.join(d,'controlDict')) and
         _op.isfile(_op.join(d,'cellModels'))):
      config_dir = d
      break
if config_dir is None:
   raise NoConfigDirError('Failed to find a global configuration directory')

# MUST be at the end
import Caelus.path as _cp
search_path = _cp.create_app_search_path(EXEC_DIR, False)
