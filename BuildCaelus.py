#!/usr/bin/python

# ---------------------------------------------------------------------------
# Caelus 5.10
# Web:   www.caelus-cml.com
# ---------------------------------------------------------------------------

# Importing the required modules for Python
import subprocess, sys, os, glob, shutil, optparse, getopt
import Caelus


# Code name and version
code = 'Caelus'
version = '5.10'

clean = True
build = True
nprocs = 0
swak = True

if sys.platform == 'win32':
   pltfrm = True
else:
   pltfrm = False

Usage="""
BuildCaelus.py [options]
 options:
  -c          | --clean-only        skip building
  -b          | --build-only        skip cleaning
  -p <nprocs> | --parallel <nprocs> build with > 0 processors
              | --no-swak           don't build swak library and utilities
  -h          | --help              print the usage
""" 

try:
   opts, args = getopt.getopt(sys.argv[1:],"hcbp:", \
                              ["clean-only", "build-only", "parallel","no-swak"])
except getopt.GetoptError:
   print Usage
   sys.exit(2)

for opt, arg in opts:
   if opt == '-h':
      print Usage
      sys.exit()
   elif opt in ("-c", "--clean-only"):
      build = False
   elif opt in ("-b", "--build-only"):
      clean = False
   elif opt in ("-p", "--parallel"):
      nprocs = arg
      if nprocs < 1:
         sys.exit("Specify >= 1 processors")
      elif nprocs ==1 :
         nprocs = 0
   elif opt in ("--no-swak"):
      swak = False

# Starting up the meshing and solving
print "**********************************"
print "Building %s %s ..." % (code, version)
print ""

build_dirs = [Caelus.METIS_PATH, \
              Caelus.SCOTCH_PATH]

if (Caelus.SCOTCH_VERSION == '6.0.4'):
  # Build ptscotch as well
  build_dirs.append(Caelus.SCOTCH_PATH + '/src')

# Add libraries
build_dirs.append('src/libraries')

# Add applications
build_dirs.append('src/applications')

if swak:
   build_dirs.append('external/swak')
else:
   print "Not building swak"

for curr_dir in build_dirs:

   # Normalize paths to be platform agnostic
   curr_dir = os.path.normpath(curr_dir)

   # Check for existance of build directory
   if not os.path.exists(curr_dir):
      sys.exit(curr_dir + ' does not exist')

   # Get name of build directory alone
   work_dir = os.path.split(curr_dir)[1]

   # Clean
   if clean:
      print ""
      print "Cleaning \'" + work_dir + "\' directory"
      print ""
      pc = subprocess.call(['scons.py', '-f', 'SConstruct.module', \
                           '-c', 'install'], cwd=curr_dir, \
                            stdout=sys.stdout, stderr=sys.stderr, \
                            shell=pltfrm)

   # Build
   if build:
      if (work_dir == 'scotch-6.0.4' and not clean):
        print ""
        print work_dir + " cannot be built without cleaning"
        pc = subprocess.call(['scons.py', '-f', 'SConstruct.module', \
                           '-c', 'install'], cwd=curr_dir, \
                            stdout=sys.stdout, stderr=sys.stderr, \
                            shell=pltfrm)
      print ""
      print "Building \'" + work_dir + "\'"
      print ""

      # Make platforms dir for new source installs
      if not os.path.exists('platforms'):
         os.mkdir('platforms')

      build_cmd = ['scons.py', '-f', 'SConstruct.module', 'install']

      # Parallel build
      if nprocs:
         build_cmd.append('-j'+str(nprocs))
		
      pb = subprocess.call(build_cmd, cwd=curr_dir, stdout=sys.stdout, \
                           stderr=sys.stderr, shell=pltfrm)

print ""
print "Done building %s %s" % (code, version)
print "**********************************"
