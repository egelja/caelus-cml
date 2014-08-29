#! /usr/bin/python

import os, sys

## FUNCTIONS ##
# check if path is valid
def check_path(path):
  if not os.path.exists(path):
    sys.exit(' ERROR: \'' + path + '\' does not exist')

# add environment variable DO NOT USE
#def add_env_var(var, isdir=False):
#  if isdir:
#    check_path(var)
  # store name of var as string
#  var_name  = [ k for k,v in locals().iteritems() if v is var][0]
#  os.environ[var_name] = var

## MAIN ##
print("Setting up Caelus environment")
DEBUG = False

# Determine default HOME dir
USER_PATH = os.path.expanduser('~')

# Determine operating system
if os.name == 'nt':
  WHICH_OS = "windows"
  sl ='/'
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
  print " INFO: Compiler not specified, using system default"

try:
  COLOUR_BUILD
  os.environ['COLOUR_BUILD'] = COLOUR_BUILD
  print " INFO: Colourful build on"
except NameError:
  os.environ['COLOUR_BUILD'] = ''
  print " INFO: Colourful build off"

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
  MPI_PATH
  os.environ['MPI_PATH'] = MPI_PATH
  check_path(MPI_PATH)
except NameError:
  os.environ['MPI_PATH'] = os.path.join(os.environ['EXTERNAL_DIR'], WHICH_OS, 'openmpi-1.5.3')
  print " INFO: MPI_PATH not specified using Caelus version"

try:
  SCOTCH_PATH
  os.environ['SCOTCH_PATH'] = SCOTCH_PATH
  check_path(SCOTCH_PATH)
except NameError:
  os.environ['SCOTCH_PATH'] = os.path.join(os.environ['EXTERNAL_DIR'], 'scotch-5.1.12')
  print " INFO: SCOTCH_PATH not specified using Caelus version"

try:
  METIS_PATH
  os.environ['METIS_PATH'] = METIS_PATH
  check_path(METIS_PATH)
except NameError:
  os.environ['METIS_PATH'] = os.path.join(os.environ['EXTERNAL_DIR'], 'metis-5.1.0')
  print " INFO: METIS_PATH not specified using Caelus version"

BIN_PLATFORM_INSTALL = os.path.join(PROJECT_DIR, PROJECT, \
                                      'platforms', BUILD_OPTION, 'bin')
os.environ['BIN_PLATFORM_INSTALL'] = BIN_PLATFORM_INSTALL

os.environ['PATH'] = os.path.join(CAELUS_PROJECT_DIR, 'bin') \
                     + os.pathsep + BIN_PLATFORM_INSTALL + os.pathsep \
                     + os.environ['PATH']

os.environ['LIB_PLATFORM_INSTALL'] = os.path.join(PROJECT_DIR, PROJECT, \
                                      'platforms', BUILD_OPTION, 'lib')

LD_LIBRARY_PATH = os.environ['LIB_PLATFORM_INSTALL'] + os.pathsep  \
                + os.path.join(os.environ['MPI_PATH'], 'lib')  \
                + os.pathsep + os.path.join(os.environ['METIS_PATH'], 'lib')  \
                + os.pathsep + os.path.join(os.environ['SCOTCH_PATH'], 'lib')   

if WHICH_OS == "darwin":
  try:
    os.environ['DYLD_LIBRARY_PATH']
    os.environ['DYLD_LIBRARY_PATH'] = LD_LIBRARY_PATH + os.pathsep + \
                                    os.environ['DYLD_LIBRARY_PATH']
  except KeyError:                
    os.environ['DYLD_LIBRARY_PATH'] = LD_LIBRARY_PATH  
else :
  try:
    os.environ['LD_LIBRARY_PATH']
    os.environ['LD_LIBRARY_PATH'] = LD_LIBRARY_PATH + os.pathsep + \
                                    os.environ['LD_LIBRARY_PATH']
  except KeyError:                
    os.environ['LD_LIBRARY_PATH'] = LD_LIBRARY_PATH

os.environ['MPI_BUFFER_SIZE'] = str(20000000)

os.environ['LIB_SRC'] = os.path.join(PROJECT_DIR, PROJECT, \
                            'src')
check_path(os.environ['LIB_SRC'])

os.environ['CAELUS_APP'] = os.path.join(PROJECT_DIR, PROJECT, \
                            'applications')
check_path(os.environ['CAELUS_APP'])

os.environ['CAELUS_SOLVERS'] = os.path.join(PROJECT_DIR, PROJECT, \
                                'applications', 'solvers')
check_path(os.environ['CAELUS_SOLVERS'])

os.environ['CAELUS_UTILITIES'] = os.path.join(PROJECT_DIR, PROJECT, \
                                  'applications', 'utilities')
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

print "Launching Caelus in new command shell"
# Launch Caelus in new shell 
if os.name == 'nt':
  print " type exit to return to previous shell"
  os.system('cmd.exe')
else:
  print " type CTRL+D or exit to return to previous shell"
  os.system(os.environ['SHELL'])
