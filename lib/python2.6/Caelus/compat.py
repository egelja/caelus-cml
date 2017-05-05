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
#   Compatibility functions that make Python 3.x code work with 2.x
#   interpreters.
#
#-------------------------------------------------------------------------------

import sys as _sys
import os.path as _op
import os as _os
import subprocess as _subprocess

def echo(*args, **kwargs):
   """print() replacement which does not depend on the Python version"""
   if 'sep' in kwargs:
      sep = kwargs['sep']
   else:
      sep = ' '
   if 'end' in kwargs:
      end = kwargs['end']
   else:
      end = '\n'
   if 'file' in kwargs:
      file = kwargs['file']
   else:
      file = _sys.stdout
   file.write(str(sep).join(map(str, args)))
   end = str(end)
   if len(end):
      file.write(end)

# check whether Python version is < 3.0
if _sys.hexversion < 0x030000f0:
  # replace the standard functions
  import itertools
  filter = itertools.ifilter
  map = itertools.imap
  zip = itertools.izip
  repr = unicode
  range = xrange
  import HTMLParser
else:
  callable = None
  xrange = None
  import html.parser as HTMLParser

# check whether Python version is < 2.6
if _sys.hexversion < 0x020600f0:
  def _posix_relpath(path, start=_os.curdir):
    """Return a relative version of a path, POSIX version"""

    if not path:
        raise ValueError("no path specified")

    start_list = _op.abspath(start).split(_op.sep)
    path_list = _op.abspath(path).split(_op.sep)

    # Work out how much of the filepath is shared by start and path.
    i = len(_op.commonprefix([start_list, path_list]))

    rel_list = [_op.pardir] * (len(start_list)-i) + path_list[i:]
    if not rel_list:
        return _os.curdir
    return _op.join(*rel_list)

  def _nt_relpath(path, start=_os.curdir):
      """Return a relative version of a path, Windows version"""

      if not path:
          raise ValueError("no path specified")
      start_list = _op.abspath(start).split(_op.sep)
      path_list = _op.abspath(path).split(_op.sep)
      if start_list[0].lower() != path_list[0].lower():
          unc_path, rest = _op.splitunc(path)
          unc_start, rest = _op.splitunc(start)
          if bool(unc_path) ^ bool(unc_start):
              raise ValueError("Cannot mix UNC and non-UNC paths (%s and %s)"
                                                                  % (path, start))
          else:
              raise ValueError("path is on drive %s, start on drive %s"
                                                  % (path_list[0], start_list[0]))
      # Work out how much of the filepath is shared by start and path.
      for i in range(min(len(start_list), len(path_list))):
          if start_list[i].lower() != path_list[i].lower():
              break
      else:
          i += 1

      rel_list = [_op.pardir] * (len(start_list)-i) + path_list[i:]
      if not rel_list:
          return _os.curdir
      return _op.join(*rel_list)

  # provide os.path.relpath
  if _os.name == 'posix':
    _op.relpath = _posix_relpath
  elif _os.name == 'nt':
    _op.relpath = _nt_relpath
  else:
    raise Exception('No implementation of os.path.relpath available')

  # provide md5/hashlib compatibility
  import md5 as _md5
  md5 = _md5.new
else:
  import hashlib as _hashlib
  md5 = lambda x: _hashlib.md5(x.encode())

# check whether Python version is < 2.5
if _sys.hexversion < 0x020500f0:
   # there is no BaseException class
   BaseException = Exception

   # there is no subprocess.check_call() function
   def _check_call(*popernargs, **kwargs):
      retcode = _subprocess.call(*popenargs, **kwargs)
      if retcode:
         cmd = kwargs.get("args")
         if cmd is None:
            cmd = popenargs[0]
         raise _subprocess.CalledProcessError(retcode, cmd)
      return 0
   _subprocess.check_call = _check_call

# check whether Python version is < 2.7
if _sys.hexversion < 0x020700f0:
   # there is no subprocess.check_output() function
   def _check_output(*popenargs, **kwargs):
      if 'stdout' in kwargs:
         raise ValueError(
               'stdout argument not allowed, it will be overridden.')
      process = _subprocess.Popen(
            stdout=_subprocess.PIPE, *popenargs, **kwargs)
      output, unused_err = process.communicate()
      retcode = process.poll()
      if retcode:
         cmd = kwargs.get("args")
         if cmd is None:
            cmd = popenargs[0]
         raise _subprocess.CalledProcessError(retcode, cmd, output=output)
      return output
   _subprocess.check_output = _check_output
