#! /usr/bin/env python

#Copyright (C) 2014 Applied CCM
# Script to generate environment files for Caelus

import os, sys, io, getopt

## FUNCTIONS ##
# check if path is valid
def check_path(path):
  if not os.path.exists(path):
    sys.exit(' ERROR: \'' + path + '\' does not exist')

Usage="""
    caelus_setup [options]
options:
  -d | --debug       print environment variables
  -v | --verbose     print messages
  -h | --help        print the usage
""" 

## MAIN ##
try:
    opts, args = getopt.getopt(sys.argv[1:], "dhv", ['debug','help','verbose'])
except getopt.GetoptError as err:
    print err 
    sys.exit()

# initialise default values for arguments
VERBOSE = False
DEBUG = False

for o, a in opts:
    if o in ('-h', '--help'):
        print Usage
        sys.exit()
    elif o in ('-v', "--verbose"):
        VERBOSE = True
    elif o in ('-d', "--debug"):
        DEBUG = True
    else:
        assert Fales, "unhandled option"

if VERBOSE:
  print("Setting up Caelus environment")

# Determine default HOME dir
USER_PATH = os.path.expanduser('~')

# Determine operating system
if os.name == 'nt':
  WHICH_OS = "windows"
  sl ='\\'
elif os.uname()[0] == 'Linux':
  WHICH_OS = "linux"
  sl ='/'
elif os.uname()[0] == 'Darwin':
  WHICH_OS = "darwin"
  sl ='/'
else:
  sys.exit(' ERROR: Unknown operating system: ' + os.uname()[0]) 

os.environ['WHICH_OS'] = WHICH_OS
os.environ['SLASH'] = sl

if VERBOSE:    
  print(' INFO: Detected operating system: ' + WHICH_OS)

# Turn off bytecode file for caelus_conf module
sys.dont_write_bytecode = True

# Import Caelus configuration
try:
  from caelus_conf import *
except ImportError:
  sys.exit("caelus_conf.py does not exist")

# Necessary environment variables
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

# Important but not necessarily required environment variables
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

if os.name == 'nt':
  try:
    acceptable_ct_values = ['on','On','1','True','true']
    if CAELUS_TERM in acceptable_ct_values:
      CAELUS_TERM = 'True'
    else:
      CAELUS_TERM = 'False'	
  except NameError:
    CAELUS_TERM = 'False'
	
# Compound environment variables
PROJECT = PROJECT_NAME + '-' + PROJECT_VER
os.environ['PROJECT'] = PROJECT

CAELUS_PROJECT_DIR = os.path.join(PROJECT_DIR, PROJECT)                     
os.environ['CAELUS_PROJECT_DIR'] = CAELUS_PROJECT_DIR
check_path(os.environ['CAELUS_PROJECT_DIR'])

BUILD_OPTION = WHICH_OS + COMPILER_ARCH + COMPILER + PRECISION_OPTION
os.environ['BUILD_OPTION'] = BUILD_OPTION

os.environ['EXTERNAL_DIR'] = os.path.join(CAELUS_PROJECT_DIR, 'external')
check_path(os.environ['EXTERNAL_DIR'])

try:
  MPI_BIN
  os.environ['MPI_BIN'] = MPI_BIN
  check_path(MPI_BIN)
  os.environ['OPAL_PREFIX'] =''
except NameError:
  os.environ['MPI_BIN'] = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5', 'bin')
  os.environ['OPAL_PREFIX'] = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5')
  if VERBOSE:
    print " INFO: MPI_BIN not specified using Caelus version"

try:
  MPI_LIB
  os.environ['MPI_LIB'] = MPI_LIB
  check_path(MPI_LIB)
except NameError:
  os.environ['MPI_LIB'] = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5','lib')
  if VERBOSE:
    print " INFO: MPI_LIB not specified using Caelus version"

try:
  MPI_INC
  os.environ['MPI_INC'] = MPI_INC
  check_path(MPI_INC)
except NameError:
  os.environ['MPI_INC'] = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.6.5','include')
  if VERBOSE:
    print " INFO: MPI_INC not specified using Caelus version"

try:
  MPI_LIB_NAME
  os.environ['MPI_LIB_NAME'] = MPI_LIB_NAME
except NameError:
  if os.name == 'nt':
    if VERBOSE:
      print " INFO: MPI_LIB_NAME not specified using Caelus version"
    os.environ['MPI_LIB_NAME'] = 'mpich2'
  else:
    if VERBOSE: 
      print " INFO: MPI_LIB_NAME not specified using Caelus version"
    os.environ['MPI_LIB_NAME'] = 'mpi'  
    
try:
  SCOTCH_PATH
  os.environ['SCOTCH_PATH'] = SCOTCH_PATH
  check_path(SCOTCH_PATH)
except NameError:
  os.environ['SCOTCH_PATH'] = os.path.join(os.environ['EXTERNAL_DIR'], 'scotch-5.1.12')
  if VERBOSE:
    print " INFO: SCOTCH_PATH not specified using Caelus version"

try:
  METIS_PATH
  os.environ['METIS_PATH'] = METIS_PATH
  check_path(METIS_PATH)
except NameError:
  os.environ['METIS_PATH'] = os.path.join(os.environ['EXTERNAL_DIR'], 'metis-5.1.0')
  if VERBOSE:
    print " INFO: METIS_PATH not specified using Caelus version"

try:
  ZLIB_PATH
  os.environ['ZLIB_PATH'] = ZLIB_PATH
  check_path(ZLIB_PATH)
except NameError:
  if os.name == 'nt':
    if VERBOSE:
      print " INFO: ZLIB_PATH not specified using Caelus version"
    os.environ['ZLIB_PATH'] = os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'zlib-1.2.7')
  else:
    if VERBOSE: 
      print " INFO: ZLIB_PATH not specified using system version"
    os.environ['ZLIB_PATH'] =''

try:
  FLEXXX_PATH
  os.environ['FLEXXX_PATH'] = FLEXXX_PATH
  check_path(FLEXXX_PATH)
except NameError:
  if os.name == 'nt':
    if VERBOSE:
      print " INFO: FLEXXX_PATH not specified using Caelus version"
    os.environ['FLEXXX_PATH'] = os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'flex-2.5.4a-1')
  else:
    if VERBOSE: 
      print " INFO: FLEXXX_PATH not specified using system version"
    os.environ['FLEXXX_PATH'] =''

SCONS_PATH = os.path.join(os.environ['EXTERNAL_DIR'], 'scons-local-2.3.4')
		
BIN_PLATFORM_INSTALL = os.path.join(PROJECT_DIR, PROJECT, \
                                      'platforms', BUILD_OPTION, 'bin')
os.environ['BIN_PLATFORM_INSTALL'] = BIN_PLATFORM_INSTALL

os.environ['LIB_PLATFORM_INSTALL'] = os.path.join(PROJECT_DIR, PROJECT, \
                                      'platforms', BUILD_OPTION, 'lib')
									  
LD_LIBRARY_PATH = os.environ['LIB_PLATFORM_INSTALL'] + os.pathsep  \
                + os.environ['MPI_LIB']  \
                + os.pathsep + os.path.join(os.environ['METIS_PATH'], 'lib')  \
                + os.pathsep + os.path.join(os.environ['SCOTCH_PATH'], 'lib')   

if WHICH_OS == "darwin":
  try:
    os.environ['DYLD_LIBRARY_PATH']
    os.environ['DYLD_LIBRARY_PATH'] = LD_LIBRARY_PATH + os.pathsep + \
                                    os.environ['DYLD_LIBRARY_PATH']
    FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH + os.pathsep + '$DYLD_LIBRARY_PATH'
  except KeyError:                
    os.environ['DYLD_LIBRARY_PATH'] = LD_LIBRARY_PATH
    FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH  
elif WHICH_OS == "windows":
  FULL_LD_LIBRARY_PATH = ''
else:
  try:
    os.environ['LD_LIBRARY_PATH']
    os.environ['LD_LIBRARY_PATH'] = LD_LIBRARY_PATH + os.pathsep + \
                                    os.environ['LD_LIBRARY_PATH']
    FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH + os.pathsep + '$LD_LIBRARY_PATH'
  except KeyError:                
    os.environ['LD_LIBRARY_PATH'] = LD_LIBRARY_PATH
    FULL_LD_LIBRARY_PATH = LD_LIBRARY_PATH

PATH = os.path.join(CAELUS_PROJECT_DIR, 'bin') \
                     + os.pathsep + BIN_PLATFORM_INSTALL + os.pathsep \
                     + os.environ['MPI_BIN'] + os.pathsep + SCONS_PATH

if os.name == 'nt':
  if CAELUS_TERM == "True":
    PATH = PATH + os.pathsep + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'mingw64', 'bin') \
         + os.pathsep + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'terminal', 'ansicon', 'x64') \
		 + os.pathsep + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'terminal', 'bin') \
		 + os.pathsep + LD_LIBRARY_PATH \
		 + os.pathsep + '%PATH%'
  else:  
    PATH = PATH + os.pathsep + os.path.join(os.environ['EXTERNAL_DIR'], 'windows', 'mingw64', 'bin') \
		 + os.pathsep + LD_LIBRARY_PATH \
         + os.pathsep + '%PATH%'

else:
  PATH = PATH + os.pathsep + '$PATH'

os.environ['MPI_BUFFER_SIZE'] = str(20000000)

os.environ['LIB_SRC'] = os.path.join(PROJECT_DIR, PROJECT, \
                            'src','libraries')
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

os.environ['CAELUS_TUTORIALS'] = os.path.join(PROJECT_DIR, PROJECT, \
                                  'tutorials')
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

# Write environment files
#Data for substitution
d={ 
'WHICH_OS':os.environ['WHICH_OS'], 
'SLASH':os.environ['SLASH'],
'PROJECT_NAME':os.environ['PROJECT_NAME'], 
'PROJECT_VER':os.environ['PROJECT_VER'], 
'PRECISION_OPTION':os.environ['PRECISION_OPTION'],
'COMPILER_ARCH':os.environ['COMPILER_ARCH'],
'BUILD_TYPE':os.environ['BUILD_TYPE'],
'FLEXXX':os.environ['FLEXXX'],
'PROJECT_DIR':os.environ['PROJECT_DIR'],
'COMPILER':os.environ['COMPILER'],
'COLOUR_BUILD':os.environ['COLOUR_BUILD'],
'PROJECT':os.environ['PROJECT'],
'CAELUS_PROJECT_DIR':os.environ['CAELUS_PROJECT_DIR'],
'BUILD_OPTION':os.environ['BUILD_OPTION'],
'EXTERNAL_DIR':os.environ['EXTERNAL_DIR'],
'MPI_INC':os.environ['MPI_INC'],
'MPI_LIB':os.environ['MPI_LIB'],
'MPI_LIB_NAME':os.environ['MPI_LIB_NAME'],
'OPAL_PREFIX':os.environ['OPAL_PREFIX'],
'SCOTCH_PATH':os.environ['SCOTCH_PATH'],
'METIS_PATH':os.environ['METIS_PATH'],
'ZLIB_PATH':os.environ['ZLIB_PATH'],
'FLEXXX_PATH':os.environ['FLEXXX_PATH'],
'BIN_PLATFORM_INSTALL':os.environ['BIN_PLATFORM_INSTALL'],
'PATH':PATH,
'LIB_PLATFORM_INSTALL':os.environ['LIB_PLATFORM_INSTALL'],
'LD_LIBRARY_PATH':FULL_LD_LIBRARY_PATH,
'MPI_BUFFER_SIZE':os.environ['MPI_BUFFER_SIZE'],
'LIB_SRC':os.environ['LIB_SRC'],
'CAELUS_APP':os.environ['CAELUS_APP'],
'CAELUS_SOLVERS':os.environ['CAELUS_SOLVERS'],
'CAELUS_UTILITIES':os.environ['CAELUS_UTILITIES'],
'CAELUS_TUTORIALS':os.environ['CAELUS_TUTORIALS'],
'CAELUS_JOB_DIR':os.environ['CAELUS_JOB_DIR'],
'CAELUS_USER_DIR':os.environ['CAELUS_USER_DIR'],
'CAELUS_RUN':os.environ['CAELUS_RUN'],
'CAELUS_USER_APPBIN':os.environ['CAELUS_USER_APPBIN'],
'CAELUS_USER_LIBBIN':os.environ['CAELUS_USER_LIBBIN']
}


if os.name == 'nt':
  from string import Template

  #Open the file
  filein = open( os.path.join(PROJECT_DIR, PROJECT, 'etc/config/caelus-environment-template') )

  #read it
  src = Template( filein.read() )

  #Do the substitution
  result = src.substitute(d)
  
  # Create the new config file for writing
  config = io.open(os.path.join(PROJECT_DIR, PROJECT, 'etc/caelus-environment.cmd'), 'w')

  # Write
  config.write(unicode(result))

  # Close the files
  config.close()
else:
  from string import Template

  class MyTemplate(Template):  
    delimiter = '&'
    idpattern = r'[a-z][_a-z0-9]*'

  #Open the file
  filein = open( os.path.join(PROJECT_DIR, PROJECT, 'etc/config/caelus-bashrc-template') )

  #read it
  src = MyTemplate( filein.read() )

  #Do the substitution
  result = src.substitute(d)
  
  # Create the new config file for writing
  config = io.open(os.path.join(PROJECT_DIR, PROJECT, 'etc/caelus-bashrc'), 'w')

  # Write
  config.write(unicode(result))

  # Close the files
  config.close()
  
########################################################################
