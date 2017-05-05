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
#   Path searching functions.
#
#-------------------------------------------------------------------------------

"""Path manipulation and search functions."""

# want to be future proof
from Caelus.compat import *
import Caelus as _Caelus

# local helper
def _isreadable(fname):
   """Determine whether a file is readable by the user.

   Parameters
   ----------
   fname : Path to the file to test.

   Returns
   -------
   True if the file is readable by the user, False otherwise.

   """
   import os
   return os.access(fname, os.R_OK)

# local helper
def _isexecutable(fname):
   """Determine whether a file is executable by the user.

   Parameters
   ----------
   fname : Path to the file to test.

   Returns
   -------
   True if the file is executable by the user, False otherwise.

   """
   import os
   import os.path
   return os.path.isfile(fname) and os.access(fname, os.X_OK)


def split_search_path(path_str, normpath=True):
   """Splits a search path string into a list of directories.

   Parameters
   ----------
   path_str : The search path to split.
   normpath : If True, os.path.normpath is applied to each element in the
              returned list.

   Returns
   -------
   path : A list of directories created by splitting 'path_str'.

   """
   import os
   import os.path
   # replace all os.pathsep by :
   if os.pathsep != ':':
      path_str = path_str.replace(os.pathsep,':')
   # split at ':' and  remove empty elements (e.g. from double :)
   path = list(filter(len, path_str.split(':')))
   # normalize paths if requested
   if normpath and len(path):
      path = list(map(os.path.normpath, path))
   return path

def find_on_search_path(fname, path, predicate=_isreadable):
   """Find a file on a search path subject to a predicate.

   The first match for which 'predicate' returns true, will be selected.
   It is completely up to 'predicate' to determine what a valid match is (e.g.
   it is allowed to return True for a non-existing file).

   Parameters
   ----------
   fname     : Name of the file to find.
   path      : Either a sequence of directories or a ':' (on Windows also ';')
               delimited string of directories to search.
   predicate : A function which returns True if the file meets the requirements.

   Returns
   -------
   path : The first path for which 'predicate' returned true.

   """
   import os.path
   if isinstance(path, str):
      path = split_search_path(path, True)
   # loop over all directories in path
   for dir in path:
      # construct full candidate path
      f = os.path.join(dir, fname)
      # test whether it meets the predicate
      if predicate(f):
         return os.path.normpath(f)
   # failed to find the file
   return None

def create_app_search_path(basedir=_Caelus.EXEC_DIR, syspath=False):
   """Create a search path for Caelus applications.

   First directories in the list given by the environment variable
   'CAELUS_PATH' (if it is defined) are used. The elements of 'CAELUS_PATH'
   must be separated by ':' (or optionally by ';' on Windows). Next the
   directory '<basedir>' is appended, where '<basedir>' defaults to
   'EXEC_DIR'. This can be overriden using the 'basedir' argument. If the
   argument 'syspath' is set to True, the directories in the system 'PATH'
   variable will appended to the list.

   Parameters
   ----------
   basedir : Directory in which the Caelus applications are located.
   syspath : If True, the system 'PATH' environment variable will also be
             searched.

   Returns
   -------
   path : A list of directories comprising the search path.

   """
   import os
   import os.path
   # set up the search path
   path = []
   # start with CAELUS_PATH elements if present
   if 'CAELUS_PATH' in os.environ:
      path.extend(split_search_path(os.environ['CAELUS_PATH']))
   # append with basedir
   path.append(os.path.abspath(basedir))
   # append system PATH elements if present and desired by the user
   if syspath and 'PATH' in os.environ:
      path.extend(split_search_path(os.environ['PATH']))
   # done
   return path

def locate_app(fname, spath=None):
   """Searches the search-path 'path' for an application named 'fname'.

   The name of the file searched will be constructed as
   'Caelus.EXE_PREFIX+fname'. The elements of 'spath' will be searched for a
   file with that name which is executable by the current user.

   Parameters
   ----------
   fname   : Name of the application to find.
   spath   : Sequence of directories to be used as the search path.
             If set to None, the default search path will be constructed
             by calling create_app_search_path with no arguments.

   Returns
   -------
   path : The full path to the file if it was found, None otherwise.

   """
   import os.path
   fname = os.path.normpath(fname)
   # first check whether this is a relative/absolute path
   if _isexecutable(fname):
      if not os.path.isabs(fname) and fname[0] != '.':
         fname = os.path.join('.',fname)
      return fname
   # construct default search path if none specified
   if not spath:
      spath = create_app_search_path()
   # construct correct file name
   fname = _Caelus.EXE_PREFIX + fname + _Caelus.EXE_SUFFIX
   # find the thing
   return find_on_search_path(fname, spath, _isexecutable)

def locate_config(fname):
   """Searches the filesystem for a configuration file named 'fname'.

    By default the 'CONFIG_DIR' directory is search, which can be overriden by
    the 'CAELUS_CONFIG_DIR' environment variable.

   Parameters
   ----------
   fname : Name of the configuration file to find.

   Returns
   -------
   path : The full path to the file if it was found, None otherwise.

   """
   import os
   import os.path
   if 'CAELUS_CONFIG_DIR' in os.environ:
      dir = os.environ['CAELUS_CONFIG_DIR']
   else:
      dir = _Caelus.CONFIG_DIR
   return find_on_search_path(fname, [dir])

def locate_tmpl(fname):
   """Searches the filesystem for the template file 'fname'.

   These text templates are searched for in 'DATA_DIR+"/templates"'.

   Parameters
   ----------
   fname : Name of the template file to locate.

   Returns
   -------
   path : The full path to the template file if it was found, None otherwise.

   """
   import os.path
   dir = os.path.join(_Caelus.DATA_DIR,'templates')
   return find_on_search_path(fname, [dir])
