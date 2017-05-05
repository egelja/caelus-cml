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
#   Write the Caelus environment files to etc/ directory.
#
#-------------------------------------------------------------------------------

"""Write Caelus environment files for traditional environment set up."""

import os, sys
from Caelus.compat import *
from Caelus.setup import *
from string import Template

print "Writing environment file to:\n \"{0}\"".format(os.path.join(PROJECT_DIR,
                                                  PROJECT,'etc'))
# Return OS appropriate path var name
path_var = lambda os: '%PATH%' if os == 'windows' else '$PATH'

# data for substitution
d={ 
'WHICH_OS':os.environ['WHICH_OS'], 
'SLASH':os.environ['SLASH'],
'PROJECT_NAME':os.environ['PROJECT_NAME'], 
'PROJECT_VER':os.environ['PROJECT_VER'], 
'PRECISION_OPTION':os.environ['PRECISION_OPTION'],
'COMPILER_ARCH':os.environ['COMPILER_ARCH'],
'BUILD_TYPE':os.environ['BUILD_TYPE'],
'INT_TYPE':os.environ['INT_TYPE'],
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
'SCOTCH_VERSION':os.environ['SCOTCH_VERSION'],
'SCOTCH_PATH':os.environ['SCOTCH_PATH'],
'METIS_PATH':os.environ['METIS_PATH'],
'ZLIB_PATH':os.environ['ZLIB_PATH'],
'FLEXXX_PATH':os.environ['FLEXXX_PATH'],
'BIN_PLATFORM_INSTALL':os.environ['BIN_PLATFORM_INSTALL'],
'PATH':PATH + os.pathsep + path_var(WHICH_OS),
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

if WHICH_OS == 'windows':
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
elif WHICH_OS == "darwin":
   class MyTemplate(Template):  
      delimiter = '&'
      idpattern = r'[a-z][_a-z0-9]*'

   #Open the bashrc template file
   filein = open( os.path.join(PROJECT_DIR, PROJECT, 'etc/config/caelus-bashrc-template-darwin') )
   #read it
   src = MyTemplate( filein.read() )
   #close it
   filein.close()
   #Do the substitution
   result = src.substitute(d)
   # Create the new config file for writing
   config = io.open(os.path.join(PROJECT_DIR, PROJECT, 'etc/caelus-bashrc'), 'w')
   # Write
   config.write(unicode(result))
   # Close the files
   config.close()

   #Open the cshrc template file
   filein = open( os.path.join(PROJECT_DIR, PROJECT, 'etc/config/caelus-cshrc-template-darwin') )
   #read it
   src = MyTemplate( filein.read() )
   #close it
   filein.close()
   #Do the substitution
   result = src.substitute(d)
   # Create the new config file for writing
   config = io.open(os.path.join(PROJECT_DIR, PROJECT, 'etc/caelus-cshrc'), 'w')
   # Write
   config.write(unicode(result))
   # Close the files
   config.close()
else:
   class MyTemplate(Template):  
      delimiter = '&'
      idpattern = r'[a-z][_a-z0-9]*'

   #Open the bashrc template file
   filein = open( os.path.join(PROJECT_DIR, PROJECT, 'etc/config/caelus-bashrc-template-linux') )
   #read it
   src = MyTemplate( filein.read() )
   #close it
   filein.close()
   #Do the substitution
   result = src.substitute(d)
   # Create the new config file for writing
   config = io.open(os.path.join(PROJECT_DIR, PROJECT, 'etc/caelus-bashrc'), 'w')
   # Write
   config.write(unicode(result))
   # Close the files
   config.close()

   #Open the cshrc template file
   filein = open( os.path.join(PROJECT_DIR, PROJECT, 'etc/config/caelus-cshrc-template-linux') )
   #read it
   src = MyTemplate( filein.read() )
   #close it
   filein.close()
   #Do the substitution
   result = src.substitute(d)
   # Create the new config file for writing
   config = io.open(os.path.join(PROJECT_DIR, PROJECT, 'etc/caelus-cshrc'), 'w')
   # Write
   config.write(unicode(result))
   # Close the files
   config.close()

