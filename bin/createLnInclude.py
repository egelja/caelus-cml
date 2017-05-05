import os, sys
import shutil
import getopt

Usage="""

    %s [options] <dir> 
options:
  -f | --force       force update
  -s | --silent      use 'silent' mode (do not echo command)
  -h | --help        print the usage

    Link all the source files in the <dir> into <dir>/lnInclude
                    
eg: %s -f core
"""

try:
  opts, args = getopt.getopt(sys.argv[1:], "hfs", ['help','force','silent'])
except getopt.GetoptError as err:
  print err 
  print Usage
  sys.exit(0)

# initialise default values for arguments
force = False
silent = False

for o, a in opts:
  if o in ('-h', '--help'):
    print Usage
    sys.exit()
  elif o in ('-s', "--silent"):
    silent = True
  elif o in ('-f', "--force"):
    force = True
  else:
    assert Fales, "unhandled option"

# Check user has provided a directory
if  (len(args) > 0) and (len(args) < 2):
  dst = args[0]
else:
  print "No directory or too many directories given"
  sys.exit()
	
# Determine if we are Windows or POSIX	
if os.name =='nt':
  which_os = "windows"
  not_which_os = "posix"
  sl = '\\'
else:
  which_os = "posix"
  not_which_os = "windows"
  sl = '/'

#Source directory, absolute path to library and absolute path to link 
#include directory
if dst =='.' :
  path = os.getcwd() + sl
else:
  path = os.getcwd() + sl + dst

incDir = path + sl + 'lnInclude'

# If destination does not exist create it
if not os.path.exists(incDir): 
  os.makedirs(incDir)
  if not (silent == True):
    print 'Creating directory '+ incDir 

# If destination exists and user has ask to force, then delete and re-create
elif (force==True):
  shutil.rmtree(incDir)
  os.makedirs(incDir)
  if not (silent == True):
    print 'Removing and re-creating directory '+ incDir
else:
  if not (silent == True):
    print 'lnInclude directory ' + incDir + ' already exists.'
    print """Use -f to force re-creation otherwise only modified files will be 
          updated"""

if not (silent == True):
  print 'Linking include files to '+ incDir

for (dirpath, dirnames, filenames) in os.walk(path):
  if not ((dirpath == incDir) or (dirpath == path + sl + 'OSspecific' + sl + not_which_os)
    or (dirpath == path + sl + 'OSspecific' + sl + not_which_os + sl + 'clockTime')
    or (dirpath == path + sl + 'OSspecific' + sl + not_which_os + sl + 'cpuTime')
    or (dirpath == path + sl + 'OSspecific' + sl + not_which_os + sl + 'signals')
    or (dirpath == path + sl + 'OSspecific' + sl + not_which_os + sl + 'memInfo')):
    for filename in filenames:
      if (
#          filename.endswith('.C') or filename.endswith('.cpp') or 
#          filename.endswith('.cxx') or
          filename.endswith('.h') or 
          filename.endswith('.H') or filename.endswith('.hpp') or  
          filename.endswith('.hxx') ):
        source = os.path.join(dirpath, filename)
        target = os.path.join(incDir, filename)
        if os.path.isfile(target):
          # file exists so we need to determine if it needs updating
          source_mtime = round(os.path.getmtime(source),2)
          target_mtime = round(os.path.getmtime(target),2)
          if source_mtime > target_mtime:
            if not (silent == True):
              print 'Copying '+ source + ' to ' + target
            # Note: symbolic linking on Windows requires Python 3.2 or above. 
            # Scons currently only works with the Python 2.x versions. Therefore
            # we shall just create copies of the files in the lnInclude 
            # directories for now. 
            #os.symlink(source, target)

            # copy the file as the source is newer
            shutil.copy2(source, target)  
        else:
          # copy the file as it doesn't exist
          if not (silent == True):
            print 'Copying '+ source + ' to ' + target
          shutil.copy2(source, target)		
