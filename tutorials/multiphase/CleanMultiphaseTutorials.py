#!/usr/bin/python
# ---------------------------------------------------------------------------
# Caelus 7.04
# Web:   www.caelus-cml.com
# ---------------------------------------------------------------------------

# Importing the required modules for Python
import sys, subprocess, os, getopt, fnmatch, re
import Caelus

# Split a path into a list
def splitpath(path, maxdepth=20):
     ( head, tail ) = os.path.split(path)
     return splitpath(head, maxdepth - 1) + [ tail ] \
         if maxdepth and head and head != path \
         else [ head or tail ]

# Code name and version
code = 'Caelus'
version = Caelus.PROJECT_VER

val_tuts = False

if sys.platform == 'win32':
   pltfrm = True
else:
   pltfrm = False

Usage="""
CleanMultiphaseTutorials.py [options]
 options:
  -v          | --validation-cases  clean validation cases (much longer run time)
  -h          | --help              print the usage
""" 

try:
   opts, args = getopt.getopt(sys.argv[1:],"hv", \
                              ["validation-cases"])
except getopt.GetoptError:
   print Usage
   sys.exit(2)

for opt, arg in opts:
   if opt == '-h':
      print Usage
      sys.exit()
   elif opt in ("-v", "--validation-cases"):
      val_tuts = True

# Walk the tutorial directory to find run scripts
tut_dirs = []
for root, dirnames, filenames in os.walk('../multiphase'):
   for filename in fnmatch.filter(filenames, 'Allclean.py'):
      tut_dirs.append(os.path.join(root, filename))

# Starting up the meshing and solving
print "**********************************"
print "Cleanning %s %s tutorials ..." % (code, version)
print ""

tut_dirs.sort(reverse=True)

for tut in tut_dirs:

   # Normalize paths to be platform agnostic
   tut = os.path.normpath(tut)

   # Get name of build directory alone
   tut_dir = os.path.split(tut)[0]
   tut_name = splitpath(tut_dir)[-1]
   script = os.path.split(tut)[1]

   if re.search('^ACCM', tut_name) and not val_tuts:
      continue

   subprocess.call(['python', script], cwd=tut_dir, shell=pltfrm, \
                    stdout=sys.stdout, stderr=sys.stderr)

print ""
print "Done cleaning %s %s tutorials" % (code, version)
print "**********************************"
