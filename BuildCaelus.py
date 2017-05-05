#!/usr/bin/python

# ---------------------------------------------------------------------------
# Caelus 4.10
# Web:   www.caelus-cml.com
# ---------------------------------------------------------------------------

# Importing the required modules for Python
import subprocess, sys, os, glob, shutil, optparse, getopt

# Code name and version
code = 'Caelus'
version = '4.10'

clean = True
build = True
nprocs = 0

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
  -h          | --help              print the usage
""" 

try:
   opts, args = getopt.getopt(sys.argv[1:],"hcbp:", \
                              ["clean-only", "build-only", "parallel"])
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

# Starting up the meshing and solving
print "**********************************"
print "Building %s %s ..." % (code, version)
print ""

build_dirs = ['external/metis-5.1.0', \
              'external/scotch-5.1.12', \
              'src/libraries', \
              'src/applications']

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
      pc = subprocess.call(['scons.py', '-c'], cwd=curr_dir, \
                            stdout=sys.stdout, stderr=sys.stderr, \
                            shell=pltfrm)

   # Build
   if build:
      print ""
      print "Building \'" + work_dir + "\'"
      print ""

      # Parallel build
      if nprocs:
         pb = subprocess.call(['scons.py', '-j'+str(nprocs), 'install'], \
		                         cwd=curr_dir, stdout=sys.stdout, \
		                         stderr=sys.stderr, shell=pltfrm)
		
      # Serial build
      else:
         pb = subprocess.call(['scons.py', 'install'], cwd=curr_dir, \
		                         stdout=sys.stdout, stderr=sys.stderr, \
		                         shell=pltfrm)

print ""
print "Done building %s %s" % (code, version)
print "**********************************"
