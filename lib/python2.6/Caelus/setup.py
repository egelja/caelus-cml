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
#    Generate environment variables and files for Caelus
#-------------------------------------------------------------------------------

"""Setup Caelus environment from configuration""" 

import os, sys, io, getopt

# Default values
VERBOSE = False
DEBUG = False

# Turn off bytecode file for conf module
sys.dont_write_bytecode = True

# Import configuration
try:
   from Caelus.conf import *
except ImportError:
   sys.exit("Configuration file does not exist")

# check if path is valid
def check_path(path):
   if not os.path.exists(path):
      sys.exit(' ERROR: \'' + path + '\' does not exist')

if VERBOSE:
   print("Setting up Caelus environment")

# Determine default HOME dir
USER_PATH = os.path.expanduser('~')

# Determine operating system
if os.name == 'nt':
   WHICH_OS = "windows"
   SLASH ='\\'
elif os.uname()[0] == 'Linux':
   WHICH_OS = "linux"
   SLASH ='/'
elif os.uname()[0] == 'Darwin':
   WHICH_OS = "darwin"
   SLASH ='/'
else:
   sys.exit(' ERROR: Unknown operating system: ' + os.uname()[0]) 

os.environ['WHICH_OS'] = WHICH_OS
os.environ['SLASH'] = SLASH

if VERBOSE:    
   print(' INFO: Detected operating system: ' + WHICH_OS)

##
# Environment variables defined in conf module
##

# Necessary
try:
   PROJECT_NAME
   os.environ['PROJECT_NAME'] = PROJECT_NAME
except NameError:
   sys.exit(' PROJECT_NAME not specified')

try:
   PROJECT_VER
   os.environ['PROJECT_VER'] = PROJECT_VER
except NameError:
   sys.exit(' PROJECT_VAR not specified')

try:
   PRECISION_OPTION
   os.environ['PRECISION_OPTION'] = PRECISION_OPTION
   if PRECISION_OPTION != 'SP' and PRECISION_OPTION != 'DP':
      sys.exit(' ERROR: Valid \'PRECISION_OPTION\' is \'SP\' or \'DP\'')
except NameError:
   sys.exit(' PRECISION_OPTION not specified')

try:
   COMPILER_ARCH
   os.environ['COMPILER_ARCH'] = COMPILER_ARCH
   if int(COMPILER_ARCH) != 32 and int(COMPILER_ARCH) != 64:
      sys.exit(' ERROR: Valid \'COMPILER_ARCH\' is \'32\' or \'64\'')
except NameError:
   sys.exit(' COMPILER_ARCH not specified')

try:
   BUILD_TYPE
   os.environ['BUILD_TYPE'] = BUILD_TYPE
   if BUILD_TYPE != 'Opt' and BUILD_TYPE != 'Debug' and BUILD_TYPE != 'Prof':
      sys.exit(' ERROR: Valid \'BUILD_TYPE\' is \'Opt\', \'Debug\' or \'Prof\'')
except NameError:
   sys.exit(' BUILD_TYPE not specified')

try:
   INT_TYPE
   os.environ['INT_TYPE'] = INT_TYPE
   if INT_TYPE != '64' and INT_TYPE != '32':
      sys.exit(' ERROR: Valid \'INT_TYPE\' is \'64\' or \'32\'')
except NameError:
   os.environ['INT_TYPE'] = '32'

try:
   OMP
   os.environ['OMP'] = OMP
   if OMP != 'True' and OMP != 'False':
      sys.exit(' ERROR: Valid \'OMP\' is \'True\' or \'False\'')
except NameError:
   os.environ['OMP'] = 'False'

try:
   FLEXXX
   os.environ['FLEXXX'] = FLEXXX
   if FLEXXX != 'flex':
      sys.exit(' ERROR: Valid \'FLEXXX\' is \'flex\'')
except NameError:
   sys.exit(' FLEXXX not specified')

try:
   PROJECT_DIR
   os.environ['PROJECT_DIR'] = PROJECT_DIR
   check_path(PROJECT_DIR)
except NameError:
   sys.exit(' PROJECT_DIR not specified')

# Important but not required
try:
   COMPILER
   os.environ['COMPILER'] = COMPILER
except NameError:
   COMPILER = 'System'
   os.environ['COMPILER'] = COMPILER
   if VERBOSE:
      print " INFO: Compiler not specified, using system default"

try:
   COLOUR_BUILD
   acceptable_on_values = ['on','On','1','True','true']
   if COLOUR_BUILD in acceptable_on_values: 
      os.environ['COLOUR_BUILD'] = 'on'
   else:
      os.environ['COLOUR_BUILD'] = 'off'
   if VERBOSE:
      print " INFO: Colourful build on"
except NameError:
   COLOUR_BUILD = 'off'
   os.environ['COLOUR_BUILD'] = 'off'
   if VERBOSE:
      print " INFO: Colourful build off"

if WHICH_OS == 'windows':
   try:
      acceptable_ct_values = ['on','On','1','True','true']
      if CAELUS_TERM in acceptable_ct_values:
         CAELUS_TERM = 'True'
      else:
         CAELUS_TERM = 'False'	
   except NameError:
      CAELUS_TERM = 'False'

##
# Environment variables constructed by this module
##

# Compound environment variables
PROJECT = PROJECT_NAME + '-' + PROJECT_VER
os.environ['PROJECT'] = PROJECT

CAELUS_PROJECT_DIR = os.path.join(PROJECT_DIR, PROJECT)                     
os.environ['CAELUS_PROJECT_DIR'] = CAELUS_PROJECT_DIR
check_path(os.environ['CAELUS_PROJECT_DIR'])

BUILD_OPTION = WHICH_OS + COMPILER_ARCH + COMPILER + PRECISION_OPTION + BUILD_TYPE
os.environ['BUILD_OPTION'] = BUILD_OPTION

EXTERNAL_DIR = os.path.join(CAELUS_PROJECT_DIR, 'external')
os.environ['EXTERNAL_DIR'] = EXTERNAL_DIR
check_path(os.environ['EXTERNAL_DIR'])

# MPI may be define in conf module
CAELUS_MPI = False
try:
   MPI_BIN
   os.environ['MPI_BIN'] = MPI_BIN
   check_path(MPI_BIN)
   os.environ['OPAL_PREFIX'] =''
except NameError:
   os.environ['MPI_BIN'] = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5', 'bin')
   os.environ['OPAL_PREFIX'] = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5')
   CAELUS_MPI = True
   if VERBOSE:
     print " INFO: MPI_BIN not specified using Caelus version"

try:
   MPI_LIB_PATH
   os.environ['MPI_LIB_PATH'] = MPI_LIB_PATH
   check_path(MPI_LIB_PATH)
except NameError:
   MPI_LIB_PATH = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5','lib')
   os.environ['MPI_LIB_PATH'] = MPI_LIB_PATH
   if VERBOSE:
      print " INFO: MPI_LIB_PATH not specified using Caelus version"

try:
   MPI_INC_PATH
   os.environ['MPI_INC_PATH'] = MPI_INC_PATH
   check_path(MPI_INC_PATH)
except NameError:
   MPI_INC_PATH = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5','include')
   os.environ['MPI_INC_PATH'] = MPI_INC_PATH 
   if VERBOSE:
      print " INFO: MPI_INC_PATH not specified using Caelus version"

try:
   MPI_LIB_NAME
   os.environ['MPI_LIB_NAME'] = MPI_LIB_NAME
except NameError:
   if WHICH_OS == 'windows':
      if VERBOSE:
         print " INFO: MPI_LIB_NAME not specified using Caelus version"
      MPI_LIB_NAME = 'mpich2'
      os.environ['MPI_LIB_NAME'] = MPI_LIB_NAME
   else:
      if VERBOSE: 
         print " INFO: MPI_LIB_NAME not specified using Caelus version"
      MPI_LIB_NAME = 'mpi'
      os.environ['MPI_LIB_NAME'] = MPI_LIB_NAME

# Scotch version may be defined in conf module
try:
   SCOTCH_VERSION
   os.environ['SCOTCH_VERSION'] = SCOTCH_VERSION
except NameError:
   SCOTCH_VERSION = '6.0.4'
   os.environ['SCOTCH_VERSION'] = SCOTCH_VERSION
   if VERBOSE:
      print " INFO: SCOTCH_VERSION not specified using Caelus version"

# Scotch path may be defined in conf module
try:
   SCOTCH_PATH
   os.environ['SCOTCH_PATH'] = SCOTCH_PATH
   check_path(SCOTCH_PATH)
except NameError:
   SCOTCH_PATH = os.path.join(os.environ['EXTERNAL_DIR'], 'scotch-'+SCOTCH_VERSION )
   os.environ['SCOTCH_PATH'] = SCOTCH_PATH
   if VERBOSE:
      print " INFO: SCOTCH_PATH not specified using Caelus version"

# Metis may be defined in conf module
try:
   METIS_PATH
   os.environ['METIS_PATH'] = METIS_PATH
   check_path(METIS_PATH)
except NameError:
   METIS_PATH = os.path.join(os.environ['EXTERNAL_DIR'], 'metis-5.1.0')
   os.environ['METIS_PATH'] = METIS_PATH 
   if VERBOSE:
      print " INFO: METIS_PATH not specified using Caelus version"

# zlib may be defined in conf module
try:
   ZLIB_PATH
   os.environ['ZLIB_PATH'] = ZLIB_PATH
   check_path(ZLIB_PATH)
except NameError:
   if WHICH_OS == 'windows':
      if VERBOSE:
         print " INFO: ZLIB_PATH not specified using Caelus version"
      ZLIB_PATH = os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'zlib-1.2.7')
      os.environ['ZLIB_PATH'] = ZLIB_PATH 
   else:
      if VERBOSE: 
         print " INFO: ZLIB_PATH not specified using system version"
      ZLIB_PATH = ''
      os.environ['ZLIB_PATH'] = ZLIB_PATH

# flex may be defined in conf module
try:
   FLEXXX_PATH
   os.environ['FLEXXX_PATH'] = FLEXXX_PATH
   check_path(FLEXXX_PATH)
except NameError:
   if WHICH_OS == 'windows':
      if VERBOSE:
         print " INFO: FLEXXX_PATH not specified using Caelus version"
      FLEXXX_PATH = os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'flex-2.5.4a-1')
      os.environ['FLEXXX_PATH'] = FLEXXX_PATH 
   else:
      if VERBOSE: 
         print " INFO: FLEXXX_PATH not specified using system version"
      FLEXXX_PATH =''
      os.environ['FLEXXX_PATH'] = FLEXXX_PATH 

# More compound environment variables
SCONS_PATH = os.path.join(os.environ['EXTERNAL_DIR'], 'scons-local-2.3.4')
		
BIN_PLATFORM_INSTALL = os.path.join(PROJECT_DIR, PROJECT, \
                        'platforms', BUILD_OPTION, 'bin')
os.environ['BIN_PLATFORM_INSTALL'] = BIN_PLATFORM_INSTALL

LIB_PLATFORM_INSTALL = os.path.join(PROJECT_DIR, PROJECT, \
                        'platforms', BUILD_OPTION, 'lib')
os.environ['LIB_PLATFORM_INSTALL'] = LIB_PLATFORM_INSTALL
 
LD_LIBRARY_PATH = os.environ['LIB_PLATFORM_INSTALL'] + os.pathsep  \
                + os.environ['MPI_LIB_PATH']  \
                + os.pathsep + os.path.join(os.environ['METIS_PATH'], 'lib')  \
                + os.pathsep + os.path.join(os.environ['SCOTCH_PATH'], 'lib')
   
# If user has defiend a zlib path add to LD_LIBRARY
if ZLIB_PATH !='':
   LD_LIBRARY_PATH = LD_LIBRARY_PATH + os.pathsep + os.path.join(os.environ['ZLIB_PATH'], 'lib')

if WHICH_OS == "darwin":
   try:
      os.environ['DYLD_FALLBACK_LIBRARY_PATH']
   except KeyError:
      os.environ['DYLD_FALLBACK_LIBRARY_PATH'] = LD_LIBRARY_PATH
      FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH
   else:
      os.environ['DYLD_FALLBACK_LIBRARY_PATH'] = LD_LIBRARY_PATH \
         + os.pathsep + os.environ['DYLD_FALLBACK_LIBRARY_PATH']
      FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH + os.pathsep \
                             + '$DYLD_FALLBACK_LIBRARY_PATH'
  
elif WHICH_OS == "windows":
   FULL_LD_LIBRARY_PATH = ''

else:
   try:
      os.environ['LD_LIBRARY_PATH']
   except KeyError:                
      os.environ['LD_LIBRARY_PATH'] = LD_LIBRARY_PATH
      FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH
   else:
      os.environ['LD_LIBRARY_PATH'] = LD_LIBRARY_PATH + os.pathsep \
                                      + os.environ['LD_LIBRARY_PATH']
      FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH + os.pathsep + '$LD_LIBRARY_PATH'

# Add platform bin directory to execution path
PATH = BIN_PLATFORM_INSTALL
# Add Caelus provided MPI bin if used
if CAELUS_MPI:
   PATH = PATH + os.pathsep + os.environ['MPI_BIN']

if WHICH_OS == 'windows':
   if CAELUS_TERM == "True":
      PATH = PATH + os.pathsep \
      + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'mingw64', 'bin') \
      + os.pathsep \
      + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'terminal', 'ansicon', 'x64') \
	   + os.pathsep \
	   + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'terminal', 'bin') \
	   + os.pathsep + LD_LIBRARY_PATH
   else:  
      PATH = PATH + os.pathsep \
      + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'mingw64', 'bin') \
		+ os.pathsep + LD_LIBRARY_PATH

# Prepend execution path to existing
os.environ['PATH'] = PATH + os.pathsep + os.environ['PATH']

# Add local site_scons to global ...
SITE_SCONS_DIR = '\"--site-dir={0}\"'.format(
                     os.path.join(CAELUS_PROJECT_DIR,'site_scons')
                  )
# ... and override existing values
os.environ['SCONSFLAGS'] = SITE_SCONS_DIR

# Main environment variables
os.environ['MPI_BUFFER_SIZE'] = str(20000000)

LIB_SRC = os.path.join(PROJECT_DIR, PROJECT, \
                         'src','libraries')
os.environ['LIB_SRC'] = LIB_SRC
check_path(os.environ['LIB_SRC'])

os.environ['CAELUS_APP'] = os.path.join(PROJECT_DIR, PROJECT, \
                            'src','applications')
check_path(os.environ['CAELUS_APP'])

os.environ['CAELUS_SOLVERS'] = os.path.join(PROJECT_DIR, PROJECT, \
                                'src','applications', 'solvers')
check_path(os.environ['CAELUS_SOLVERS'])

os.environ['CAELUS_UTILITIES'] = os.path.join(PROJECT_DIR, PROJECT, \
                                  'src','applications', 'utilities')
check_path(os.environ['CAELUS_UTILITIES'])

CAELUS_TUTORIALS = os.path.join(PROJECT_DIR, PROJECT, \
                                  'tutorials')
os.environ['CAELUS_TUTORIALS'] = CAELUS_TUTORIALS
check_path(os.environ['CAELUS_TUTORIALS'])

os.environ['CAELUS_JOB_DIR'] = os.path.join(PROJECT_DIR, PROJECT, \
                                'jobControl')

# User environment variables
os.environ['CAELUS_USER_DIR'] = os.path.join(USER_PATH, PROJECT_NAME, \
                                 PROJECT)

os.environ['CAELUS_RUN'] = os.path.join(USER_PATH, PROJECT_NAME, \
                            PROJECT, 'run')

os.environ['CAELUS_USER_APPBIN'] = os.path.join(USER_PATH, PROJECT_NAME, \
                                    PROJECT, 'platforms', BUILD_OPTION, \
                                    'bin')

os.environ['CAELUS_USER_LIBBIN'] = os.path.join(USER_PATH, PROJECT_NAME, \
                                    PROJECT, 'platforms', BUILD_OPTION, \
                                    'lib')

# Print all environment variables
if DEBUG:
   for name, value in os.environ.items():
      print " %s = %s" % (name, value)
