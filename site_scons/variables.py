# -*- coding: utf-8 -*-

"""\
Caelus Build-time Variables
----------------------------

This module defines the SCons variables used to control the compilation process
for Caelus.

"""

import os
import getpass
from SCons.Variables import (Variables, EnumVariable, PathVariable,
                             BoolVariable)
from SCons.Script import ARGUMENTS
import utils

### General Variables
PROJECT_NAME     = "caelus"
PROJECT_VERSION  = '9.04'
METIS_VERSION    = '5.1.0'
GSL_VERSION      = '2.3'
SCOTCH_VERSION   = '6.0.4'
MPI_VERSION      = '1.6.5'
USERNAME         = getpass.getuser()

SCONS_SITE_DIR   = os.path.dirname(__file__)
PROJECT_DIR      = os.path.dirname(SCONS_SITE_DIR)
PROJECT_BASE     = os.path.dirname(PROJECT_DIR)
PROJECT_EXT_DIR  = os.path.join(PROJECT_DIR,'external')

ostype = utils.ostype()

caelus_vars = Variables(os.path.join(PROJECT_DIR, 'build_config.py'),
                        ARGUMENTS)
caelus_vars.AddVariables(
    # Project specific variables
    ('PROJECT_NAME', 'Name of the project', PROJECT_NAME),
    ('PROJECT_VERSION', 'Caelus version', PROJECT_VERSION),
    PathVariable('PROJECT_DIR', 'Path to the project directory',
                 PROJECT_DIR, PathVariable.PathIsDir),
    PathVariable('USER_DIR', 'Path to user directory',
                 os.path.join(PROJECT_BASE,
                              "%s-%s"%(USERNAME, PROJECT_VERSION)),
                 PathVariable.PathAccept),

    # Compiler settings
    ('CC', 'C compiler', os.environ.get('CC', 'gcc')),
    ('CXX', 'C++ compiler', os.environ.get('CXX', 'g++')),
    EnumVariable('BUILD_TYPE', 'Type of build', 'Opt',
                 allowed_values=('Opt','Debug','Prof')),
    EnumVariable('BUILD_ARCH', 'Build architecture', '64',
                 allowed_values=('64', '32')),
    EnumVariable('PRECISION', 'Single/Double precision','DP',
                 allowed_values=('DP','SP')),
    EnumVariable('INT_TYPE', 'Integer size', '32',
                 allowed_values=('32', '64')),
    BoolVariable('OMP', 'Use OpenMP multi-threading', False),

    # Additional dependencies
    BoolVariable('BUILD_SCOTCH', 'Build Scotch library', True),
    BoolVariable('BUILD_METIS', 'Build METIS library', True),
    BoolVariable('BUILD_GSL', 'Build GSL library', True),
    BoolVariable('BUILD_SWAK', 'Build SWAK utilities', True),
    BoolVariable('BUILD_CFMESH', 'Build cfMesh utilities', True),
    PathVariable('METIS_PATH', 'Path to the METIS library',
                 os.path.join(PROJECT_EXT_DIR,
                              "metis-%s"%METIS_VERSION),
                 PathVariable.PathIsDir),
    PathVariable('METIS_LIB_PATH', 'Path to the METIS lib files','CAELUS LIBRARY PATH',
                 PathVariable.PathAccept),
    PathVariable('METIS_INC_PATH', 'Path to the METIS headers',
                 os.path.join(PROJECT_EXT_DIR,
                              "metis-%s"%METIS_VERSION,'include'),
                 PathVariable.PathIsDir),
    PathVariable('SCOTCH_PATH', 'Path to the SCOTCH library',
                 os.path.join(PROJECT_EXT_DIR,
                              "scotch-%s"%SCOTCH_VERSION),
                 PathVariable.PathIsDir),
    PathVariable('SCOTCH_LIB_PATH', 'Path to the SCOTCH lib files','CAELUS LIBRARY PATH',
                 PathVariable.PathAccept),
    PathVariable('SCOTCH_INC_PATH', 'Path to the SCOTCH headers',
                 os.path.join(PROJECT_EXT_DIR,
                              "scotch-%s"%SCOTCH_VERSION,
                              "include"),
                 PathVariable.PathIsDir),
    ('MPI_LIB_NAME', 'MPI library name','mpi'),
    PathVariable('MPI_INC_PATH', 'Path to MPI headers',
                 os.path.join(PROJECT_EXT_DIR,ostype,
                              "openmpi-%s"%MPI_VERSION,
                              "include"),
                 PathVariable.PathIsDir),
    PathVariable('MPI_LIB_PATH', 'Path to MPI headers',
                 os.path.join(PROJECT_EXT_DIR,ostype,
                              "openmpi-%s"%MPI_VERSION,
                              "lib"),
                 PathVariable.PathIsDir),
    PathVariable('SWAK_PATH', 'Path to SWAK library',
                 os.path.join(PROJECT_EXT_DIR,
                              "swak","libraries")),
    PathVariable('CFMESH_PATH', 'Path to cfMesh library',
                 os.path.join(PROJECT_EXT_DIR,
                              "cfmesh")),
    PathVariable('GSL_PATH', 'Path to gsl library',
                 os.path.join(PROJECT_EXT_DIR,
                              "pgsl-%s"%GSL_VERSION)),
    PathVariable('GSL_LIB_PATH', 'Path to the GSL lib files','CAELUS LIBRARY PATH',
                 PathVariable.PathAccept),
    PathVariable('GSL_INC_PATH', 'Path to the GSL headers',
                 os.path.join(PROJECT_EXT_DIR,
                              "pgsl-%s"%GSL_VERSION,'include')),
)

if ostype == "windows":
    caelus_vars.AddVariables(
        ('FLEXXX', 'Flex executable', 'flex'),
        PathVariable('FLEXXX_PATH', 'Path to FLEX library (Windows)',
                     os.path.join(PROJECT_EXT_DIR,ostype,
                                 'flex-2.5.4a-1')),
        PathVariable('ZLIB_PATH', 'Path to ZLIB library (Windows)',
                     os.path.join(PROJECT_EXT_DIR,ostype,
                                 'zlib-1.2.7')),
    )
else:
    caelus_vars.AddVariables(
        PathVariable('ZLIB_PATH', 'Path to ZLIB library','',PathVariable.PathAccept)
    )


def init_dependent_vars(env):
    """Initialize dependent variables based on user configuration"""
    ostype = utils.ostype()
    prj_dir = env['PROJECT_DIR']
    prj_ver = env['PROJECT_VERSION']

    BUILD_OPTION = (ostype + env['BUILD_ARCH'] +
                    env['CXX'] + env['PRECISION'] + env['BUILD_TYPE'])
    if env['INT_TYPE'] == '64':
        LABEL_TYPE = ('Int' + env['INT_TYPE'])
        BUILD_OPTION = (ostype + env['BUILD_ARCH'] +
                        env['CXX'] + env['PRECISION'] + LABEL_TYPE +
                        env['BUILD_TYPE'])

    PLATFORM_INSTALL = os.path.join(
        prj_dir,'platforms',BUILD_OPTION)
    BIN_PLATFORM_INSTALL = os.path.join(PLATFORM_INSTALL,'bin')
    LIB_PLATFORM_INSTALL = os.path.join(PLATFORM_INSTALL,'lib')
    LIB_SRC = os.path.join(prj_dir, 'src', 'libraries')
    PROJECT_INC_DIR = os.path.join(LIB_SRC,'core','lnInclude')
    EXTERNAL_DIR = os.path.join(prj_dir,'external')
    EXTERNAL_WINDOWS_DIR = os.path.join(EXTERNAL_DIR,'windows')

    user_dir = env['USER_DIR']
    CAELUS_USER_LIB_SRC = os.path.join(user_dir, 'src', 'libraries')
    CAELUS_USER_APP_SRC = os.path.join(user_dir, 'src', 'applications')
    CAELUS_USER_APPBIN = os.path.join(user_dir, 'platforms',
                                      BUILD_OPTION, 'bin')
    CAELUS_USER_LIBBIN = os.path.join(user_dir, 'platforms',
                                      BUILD_OPTION, 'lib')
    env.Append(
        BUILD_OPTION = BUILD_OPTION,
        BIN_PLATFORM_INSTALL = BIN_PLATFORM_INSTALL,
        LIB_PLATFORM_INSTALL = LIB_PLATFORM_INSTALL,
        LIB_SRC = LIB_SRC,
        PROJECT_INC_DIR = PROJECT_INC_DIR,
        EXTERNAL_DIR = EXTERNAL_DIR,
        EXTERNAL_WINDOWS_DIR = EXTERNAL_WINDOWS_DIR,
        CAELUS_USER_LIBBIN = CAELUS_USER_LIBBIN,
        CAELUS_USER_APPBIN = CAELUS_USER_APPBIN,
        CAELUS_USER_LIB_SRC = CAELUS_USER_LIB_SRC,
        CAELUS_USER_APP_SRC = CAELUS_USER_APP_SRC,
    )

    if env['BUILD_METIS']:
        env.Replace(METIS_INC_PATH =
                    os.path.join(PROJECT_EXT_DIR,
                                 "metis-%s"%METIS_VERSION,
                                 "include"))
        env.Replace(METIS_LIB_PATH = LIB_PLATFORM_INSTALL)

    if env['BUILD_SCOTCH']:
        env.Replace(SCOTCH_INC_PATH =
                    os.path.join(PROJECT_EXT_DIR,
                                 "scotch-%s"%SCOTCH_VERSION,
                                 "include"))
        env.Replace(SCOTCH_LIB_PATH = LIB_PLATFORM_INSTALL)
