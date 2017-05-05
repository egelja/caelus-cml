#!/usr/bin/python
# Script
#     CaelusClearPolyMesh
#
# Description
#    Remove the contents of the constant/polyMesh directory
#
#------------------------------------------------------------------------------

"""Usage: CaelusClearPolyMesh [-h, -help] [-case <dir>] [-region <name>]

Remove the contents of the constant/polyMesh directory as per the
CML::polyMesh::removeFiles() method.

Options
-------
-case <dir>    The case directory from which to clear the polyMesh
-region <name> The mesh region to clear
-h, -help      Display this help message.

"""

import os
import sys

class NoPolyMeshDirectoryError(Exception):
   """Thrown if no polyMesh directory could be found"""
   def __init__(self):
      Exception.__init__(self)

   def __str__(self):
      return 'Failed to find a polyMesh directory'

def rm_rf(d):
   for path in (os.path.join(d,f) for f in os.listdir(d)):
      if os.path.isdir(path):
         rm_rf(path)
      else:
         os.unlink(path)
   os.rmdir(d)


def clear_polymesh(case=None, region=None):
   """Remove the contents of the constant/polyMesh directory as per the
   Foam::polyMesh::removeFiles() method.

   Parameters
   ----------
   case   : Case directory from which to clear the mesh
   region : Mesh region to clear.

   """
   meshDir = 'polyMesh'
   if region:
      meshDir = os.path.join(region, meshDir)
   tmp = os.path.join('constant', meshDir)
   if case:
      meshDir = os.path.join(case, tmp)
   else:
      if os.path.isdir(tmp):
         meshDir = tmp
      elif os.path.isdir(meshDir):
         # probably already in 'constant'
         pass
      elif os.path.basename(os.getcwd()) == 'polyMesh':
         # probably already in 'polyMesh'
         meshDir = os.getcwd()

   if not os.path.isdir(meshDir):
      raise NoPolyMeshDirectoryError()

   for f in """points faces owner neighbour cells boundary pointZones faceZones
   cellZones meshModifiers parallelData sets cellLevel pointLevel
   refinementHistory surfaceIndex""".split():
      f = os.path.join(meshDir, f)
      if os.path.isfile(f):
         os.remove(f)

   for fgz in """points.gz faces.gz owner.gz neighbour.gz cells.gz boundary.gz pointZones.gz faceZones.gz
   cellZones.gz meshModifiers.gz parallelData.gz sets.gz cellLevel.gz pointLevel.gz
   refinementHistory.gz surfaceIndex.gz""".split():
      fgz = os.path.join(meshDir, fgz)
      if os.path.isfile(fgz):
         os.remove(fgz)

   for d in """sets""".split():
      d = os.path.join(meshDir, d)
      if os.path.isdir(d):
         rm_rf(d)

# parse options
case = None
region = None
args = sys.argv[1:]
while len(args) > 0:
   a = args[0]
   if a == '-case':
      if len(args) < 2:
         sys.stderr.write('Error: The -case option requires an argument\n')
         sys.exit(1)
      case = args[1]
      del args[0:2]
   elif a == '-region':
      if len(args) < 2:
         sys.stderr.write('Error: The -region option requires an argument\n')
         sys.exit(1)
      region = args[1]
      del args[0:2]
   elif a == '-h' or a == '-help':
      print(__doc__)
      sys.exit(0)
   else:
      sys.stderr.write('Error: unknown argument "%s"\n'%a)
      sys.stderr.write(__doc__+'\n')
      sys.exit(1)

try:
   clear_polymesh(case, region)
except NoPolyMeshDirectoryError, e:
   sys.stderr.write('Error: '+str(e)+'\n')
   sys.exit(1)

