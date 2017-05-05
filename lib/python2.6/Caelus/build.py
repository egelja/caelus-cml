#-------------------------------------------------------------------------------
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
#    Centralize common information for building SConstruct feils
#
#-------------------------------------------------------------------------------

"""Common SCons information used to build SConstruct files"""

import os
import sys
import platform
import subprocess
import re

from Caelus.compat import *
import Caelus as _Caelus

# Need this in order to refer to objects in the SCons namespace 
from SCons.Script import *

#===============================================================================
# Define colours for pretty output (non-Windows only)
#===============================================================================
colors = {}
colors['cyan']   = '\033[1;36m'
colors['purple'] = '\033[1;35m'
colors['blue']   = '\033[1;34m'
colors['green']  = '\033[1;32m'
colors['yellow'] = '\033[1;33m'
colors['red']    = '\033[0;31m'
colors['end']    = '\033[0m'
colors['white']  = '\033[1;37m'
colors['bluebg']  = '\033[44;37m'

# If the output is not a terminal, remove the colours
if not sys.stdout.isatty():
   for key, value in colors.iteritems():
      colors[key] = ''

compile_source_message = '%sCompiling %s==> %s$SOURCE%s' % \
 (colors['blue'], colors['purple'], colors['yellow'], colors['end'])

compile_shared_source_message = '%sCompiling shared %s==> %s$SOURCE%s' % \
 (colors['blue'], colors['purple'], colors['yellow'], colors['end'])

link_program_message = '%sLinking Program %s==> %s$TARGET%s' % \
 (colors['red'], colors['purple'], colors['yellow'], colors['end'])

link_library_message = '%sLinking Static Library %s==> %s$TARGET%s' % \
 (colors['red'], colors['purple'], colors['yellow'], colors['end'])

ranlib_library_message = '%sRanlib Library %s==> %s$TARGET%s' % \
 (colors['red'], colors['purple'], colors['yellow'], colors['end'])

link_shared_library_message = '%sLinking Shared Library %s==> %s$TARGET%s' % \
 (colors['red'], colors['purple'], colors['yellow'], colors['end'])

java_library_message = '%sCreating Java Archive %s==> %s$TARGET%s' % \
 (colors['red'], colors['purple'], colors['yellow'], colors['end'])

install_library_message = '%sInstalling Library %s==> %s$TARGET%s' % \
 (colors['green'], colors['purple'], colors['yellow'], colors['end'])

#===============================================================================
# Pass operating system environment
#===============================================================================

# Default Global Environment is dependent on the type of operating system
if _Caelus.WHICH_OS == 'windows':
   globalEnv = Environment(tools = ['mingw'], ENV = os.environ)
   globalEnv.Append(OSTYPE = 'windows')
else:
   globalEnv = Environment(ENV = os.environ)
   globalEnv.Append(OSTYPE = os.name)

#===============================================================================
# Common environment variables
#===============================================================================
# Variables which libraries and modules should not change
globalEnv.Append(SLASH = _Caelus.SLASH)
globalEnv.Append(WHICH_OS = _Caelus.WHICH_OS)
globalEnv.Append(COLOUR_BUILD = _Caelus.COLOUR_BUILD)
globalEnv.Append(COMPILER_ARCH = _Caelus.COMPILER_ARCH)
globalEnv.Append(COMPILER = _Caelus.COMPILER)
globalEnv.Append(BUILD_TYPE = _Caelus.BUILD_TYPE)
globalEnv.Append(INT_TYPE = _Caelus.INT_TYPE)
globalEnv.Append(PRECISION_OPTION = _Caelus.PRECISION_OPTION)
globalEnv.Append(PROJECT = _Caelus.PROJECT)
globalEnv.Append(PROJECT_VER = _Caelus.PROJECT_VER)
globalEnv.Append(CAELUS_PROJECT_DIR = _Caelus.CAELUS_PROJECT_DIR)
globalEnv.Append(EXTERNAL_DIR = _Caelus.EXTERNAL_DIR)
globalEnv.Append(BUILD_OPTION = _Caelus.BUILD_OPTION)
globalEnv.Append(MPI_LIB = _Caelus.MPI_LIB)
globalEnv.Append(MPI_INC = _Caelus.MPI_INC)
globalEnv.Append(MPI_LIB_NAME = _Caelus.MPI_LIB_NAME)
globalEnv.Append(SCOTCH_VERSION = _Caelus.SCOTCH_VERSION)
globalEnv.Append(SCOTCH_PATH = _Caelus.SCOTCH_PATH)
globalEnv.Append(METIS_PATH = _Caelus.METIS_PATH)
globalEnv.Append(ZLIB_PATH = _Caelus.ZLIB_PATH)
globalEnv.Append(FLEXXX_PATH = _Caelus.FLEXXX_PATH)

# Variables which libraries and modules may change
globalEnv.Append(BIN_PLATFORM_INSTALL = _Caelus.BIN_PLATFORM_INSTALL)
globalEnv.Append(LIB_PLATFORM_INSTALL = _Caelus.LIB_PLATFORM_INSTALL)
globalEnv.Append(PROJECT_INC = _Caelus.LIB_SRC + '/core/lnInclude')
globalEnv.Append(LIB_SRC = _Caelus.LIB_SRC)

# Temporary directory location in Windows
if _Caelus.WHICH_OS == "windows":
   globalEnv['ENV']['TMP'] = os.environ['TMP']

# Override default Lexer flags as they don't work with the Caelus lex files
globalEnv.Replace(LEXCOM = '$LEX $LEXFLAGS -o$TARGET -f $SOURCES')

#===============================================================================
# Add colourful output
#===============================================================================
if _Caelus.COLOUR_BUILD == "on":
   globalEnv.Append(CXXCOMSTR = compile_source_message)
   globalEnv.Append(CCCOMSTR = compile_source_message)
   globalEnv.Append(SHCCCOMSTR = compile_shared_source_message)
   globalEnv.Append(SHCXXCOMSTR = compile_shared_source_message)
   globalEnv.Append(ARCOMSTR = link_library_message)
   globalEnv.Append(RANLIBCOMSTR = ranlib_library_message)
   globalEnv.Append(SHLINKCOMSTR = link_shared_library_message)
   globalEnv.Append(LINKCOMSTR = link_program_message)
   globalEnv.Append(JARCOMSTR = java_library_message)
   globalEnv.Append(JAVACCOMSTR = compile_source_message)
   globalEnv.Append(INSTALLSTR = install_library_message)

#===============================================================================
# Print system architecture information
#===============================================================================
if platform.machine().endswith('64'):
   platform_arch = '64bit'
   print ">> Architecture : " + platform_arch
else:
   platform_arch = '32bit'
   print ">> Architecture : " + platform_arch

# Warn if requested compiler architecture is different from system
if platform_arch != globalEnv['COMPILER_ARCH']+'bit' :
   print '! You have request a ', \
         _Caelus.COMPILER_ARCH+'bit','build on a', \
         platform_arch, 'system, this may not be possible'

# Build type
print ">> Build type : " + _Caelus.BUILD_TYPE

# Build architecture
print ">> Build architecture : " + _Caelus.COMPILER_ARCH + 'bit'

# Build precision
print ">> Build precision : " + _Caelus.PRECISION_OPTION

# Integer type
print ">> Interger type : " + _Caelus.INT_TYPE + 'bit'

#===============================================================================
# Common compiler parameters and flags
#===============================================================================

# Set the C++ compiler
if _Caelus.COMPILER != "System":
   globalEnv.Replace(CXX = _Caelus.COMPILER)

# Common flags

# System specific flags
if _Caelus.WHICH_OS == "windows":
# Begin windows flags

   # General
   globalEnv.Append(CXXFLAGS = '-m64')
   globalEnv.Append(CXXFLAGS = '-D' + _Caelus.WHICH_OS)
   globalEnv.Append(CXXFLAGS = '-DWM_' + _Caelus.PRECISION_OPTION)

   # Warnings
   globalEnv.Append(CXXFLAGS = '-Wall')
   globalEnv.Append(CXXFLAGS = '-Wextra')
   globalEnv.Append(CXXFLAGS = '-Wno-unused-parameter')
   globalEnv.Append(CXXFLAGS = '-Wnon-virtual-dtor')

   if _Caelus.BUILD_TYPE == 'Debug':
   # Debug
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O0')
      globalEnv.Append(CXXFLAGS = '-fdefault-inline')
      # Debug
      globalEnv.Append(CXXFLAGS = '-ggdb3')
      globalEnv.Append(CXXFLAGS = '-DFULLDEBUG')

   elif globalEnv['BUILD_TYPE'] == 'Prof':
   # Profiling
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O2')
      #Debug
      globalEnv.Append(CXXFLAGS = '-pg')

   else:
   # Optimised     
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O3')
      #Debug
      globalEnv.Append(CXXFLAGS = '')

# End windows compiler flags

elif _Caelus.WHICH_OS == "darwin" :
# Begin OSX compiler flags

   # General
   globalEnv.Append(CXXFLAGS = '-m64')
   globalEnv.Append(CXXFLAGS = '-D' + _Caelus.WHICH_OS)
   globalEnv.Append(CXXFLAGS = '-DWM_' + _Caelus.PRECISION_OPTION)
   globalEnv.Append(CXXFLAGS = '-std=c++11')

   # Warnings
   globalEnv.Append(CXXFLAGS = '-Wall')
   globalEnv.Append(CXXFLAGS = '-Wextra')
   globalEnv.Append(CXXFLAGS = '-Wno-unused-parameter')
   globalEnv.Append(CXXFLAGS = '-Wnon-virtual-dtor')
   globalEnv.Append(CXXFLAGS = '-Wold-style-cast')
   globalEnv.Append(CXXFLAGS = '-Wno-overloaded-virtual')
   globalEnv.Append(CXXFLAGS = '-Wno-unused-comparison')
   globalEnv.Append(CXXFLAGS = '-Wno-deprecated-register')

   if _Caelus.BUILD_TYPE == 'Debug':
   # Debug
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O0')
      globalEnv.Append(CXXFLAGS = '-fdefault-inline')
      # Debug
      globalEnv.Append(CXXFLAGS = '-ggdb3')
      globalEnv.Append(CXXFLAGS = '-DFULLDEBUG')

   elif globalEnv['BUILD_TYPE'] == 'Prof':
   # Profiling
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O2')
      #Debug
      globalEnv.Append(CXXFLAGS = '-pg')

   else:
   # Optimised     
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O3')
      #Debug
      globalEnv.Append(CXXFLAGS = '')

# End OSX compiler flags

else :
# Begin Linux compiler flags

   # General
   globalEnv.Append(CXXFLAGS = '-m64')
   globalEnv.Append(CXXFLAGS = '-D' + _Caelus.WHICH_OS + _Caelus.COMPILER_ARCH)
   globalEnv.Append(CXXFLAGS = '-DWM_' + _Caelus.PRECISION_OPTION)

   # Flag to turn label into 64bit int during compilation
   if  _Caelus.INT_TYPE == '64':
      globalEnv.Append(CXXFLAGS = '-DCAELUS_LABEL64')

      # Warnings
      globalEnv.Append(CXXFLAGS = '-Wall')
      globalEnv.Append(CXXFLAGS = '-Wextra')
      globalEnv.Append(CXXFLAGS = '-Wno-unused-parameter')
      globalEnv.Append(CXXFLAGS = '-Wnon-virtual-dtor')

   if _Caelus.BUILD_TYPE == 'Debug':
   # Debug
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O0')
      globalEnv.Append(CXXFLAGS = '-fdefault-inline')
      # Debug
      globalEnv.Append(CXXFLAGS = '-ggdb3')
      #globalEnv.Append(CXXFLAGS = '-DFULLDEBUG')

   elif globalEnv['BUILD_TYPE'] == 'Prof':
   # Profiling
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O2')
      #Debug
      globalEnv.Append(CXXFLAGS = '-pg')

   else:
   # Optimised     
      # Optimisation
      globalEnv.Append(CXXFLAGS = '-O3')
      #Debug
      globalEnv.Append(CXXFLAGS = '')

# End Linux compiler flags

# Parameter compiler flags
globalEnv.Append(CXXFLAGS = '-ftemplate-depth-100')

#===============================================================================
# Common include path(s)
#===============================================================================
globalEnv.Append(CPPPATH = [globalEnv['PROJECT_INC']])

#===============================================================================
# Common linker flags
#===============================================================================
if _Caelus.WHICH_OS == "windows" :
   globalEnv.Prepend(LINKFLAGS = '-Xlinker --add-needed')
   globalEnv.Prepend(LINKFLAGS = '-Xlinker --no-as-needed')

elif _Caelus.WHICH_OS == "darwin" :
   globalEnv.Prepend(LINKFLAGS = '-undefined dynamic_lookup')

else :
   globalEnv.Prepend(LINKFLAGS = '-Xlinker --add-needed')
   globalEnv.Prepend(LINKFLAGS = '-Xlinker --no-as-needed')

#===============================================================================
# Common link library path(s)
#===============================================================================
globalEnv.Append(LIBPATH = [_Caelus.LIB_PLATFORM_INSTALL])

#===============================================================================
# Common link library(ies)
#===============================================================================
#globalEnv.Append(LIBS = [])
