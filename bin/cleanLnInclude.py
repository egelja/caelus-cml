import os, sys
import shutil
import getopt

Usage="""

    %s [options] <dir> 
options:
  -s | --silent      use 'silent' mode (do not echo command)
  -h | --help        print the usage

    Remove all the lnInclude directories below <dir>
                    
eg: %s OpenFOAM
""" 

try:
    opts, args = getopt.getopt(sys.argv[1:], "hs", ['help','silent'])
except getopt.GetoptError as err:
    print (err) 
    print (Usage)
    sys.exit()

# initialise default values for arguments
silent = False

for o, a in opts:
    if o in ('-h', '--help'):
        print (Usage)
        sys.exit()
    elif o in ('-s', "--silent"):
        silent = True
    else:
        assert Fales, "unhandled option"

# Check user has provided a directory
if  (len(args) > 0) and (len(args) < 2):
    dir = args[0]
else:
    print ("No directory or too many directories given")
    sys.exit()
	
# Determine if we are Windows or POSIX	
if os.name =='nt':
    which_os = "windows"
    sl = '\\'
else:
    which_os = "posix"
    sl = '/'

# Absolute path to directory tree where the lnInclude files should be removed
if dir =='.' :
    path = os.getcwd() + sl
else:
    path = os.getcwd() + sl + dir

if not (silent == True):
  print ("Removing lnInclude directories: " + dir)

for (dirpath, dirnames, filenames) in os.walk(path):
    for dirname in dirnames:
        if (dirname == 'lnInclude'):
            shutil.rmtree(os.path.join(dirpath, dirname))
            if not (silent == True):
                print ('Removing directory '+ os.path.join(dirpath, dirname))
