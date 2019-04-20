# -*- coding: utf-8 -*-

"""\
Caelus compiler settings
-------------------------
"""

import os
import utils

cxxflags = dict(
    general = "-m64 -ftemplate-depth-100",
    warnings = "-Wall -Wextra -Wno-unused-parameter -Wnon-virtual-dtor -Wno-attributes -Wno-invalid-offsetof",
    debug = "-O0 -fdefault-inline -ggdb3 -DFULLDEBUG",
    prof = "-O2 -pg",
    opt = "-O3"
)

gcc_flags = dict(**cxxflags)

intel_flags = dict(**cxxflags)
intel_flags['warnings'] = "-wd327,654,819,1125,1292,1476,1505,1572"

clang_flags = dict(**cxxflags)
clang_flags['warnings'] = "-Wall -Wextra -Wno-unused-parameter -Wno-old-style-cast -Wno-overloaded-virtual -Wno-unused-comparison -Wno-invalid-offsetof -Wno-undefined-var-template"

#gcc_flags['warnings'] = clang_flags['warnings']

_compiler_map = {
    'gcc' : 'g++',
    'g++' : 'g++',
    'cc'  : 'c++',
    'c++' : 'c++',
    'clang' : 'clang++',
    'clang++' : 'clang++',
    'icc' : 'icpc',
    'intel' : 'icpc',
    'icpc' : 'icpc'
}

_compiler_flags_map = {
    'g++' : gcc_flags,
    'icpc': intel_flags,
    'clang++' : clang_flags,
    'c++' : clang_flags,
}

def windows_flags(env):
    """Windows specific compiler flags"""
    env.Append(CXXFLAGS = '-std=c++11')
    env.Append(
        CPPPATH = [
            env['MPI_INC_PATH'],
            os.path.join(env['ZLIB_PATH'],'include'),
            os.path.join(env['FLEXXX_PATH'],'include'),
        ],
        LIBPATH_LIBS = [
            env['MPI_LIB_PATH'],
            os.path.join(env['ZLIB_PATH'],'lib'),
        ]
    )
    env.Prepend(LINKFLAGS = '-Xlinker --add-needed')
    env.Prepend(LINKFLAGS = '-Xlinker --no-as-needed')

def linux_flags(env):
    """Linux specific compiler flags"""
    env.Append(CXXFLAGS = '-std=c++11')
    if env['INT_TYPE'] == '64':
        env.Append(CXXFLAGS = '-DCAELUS_LABEL64')
    env.Append(
        LIBPATH_COMMON = [
            env['MPI_LIB_PATH'],
            os.path.join(env['ZLIB_PATH'],'lib'),
        ],
        CPPPATH = [
            env['MPI_INC_PATH'],
            os.path.join(env['ZLIB_PATH'],'include'),
        ]
    )

    env.Prepend(LINKFLAGS = '-Xlinker --add-needed')
    env.Prepend(LINKFLAGS = '-Xlinker --no-as-needed')

def darwin_flags(env):
    """Darwin specific compiler flags"""
    env.Append(CXXFLAGS = '-std=c++11')

    env.Append(LIBPATH_COMMON = [env['MPI_LIB_PATH']],
               CPPPATH = [env['MPI_INC_PATH']],
               LIBS = ['pthread', ])

    env.Prepend(LINKFLAGS = '-undefined dynamic_lookup')

_arch_map = dict(
    windows=windows_flags,
    linux=linux_flags,
    darwin=darwin_flags,
)

def update_compiler_settings(env):
    """Update C++ compiler flags and other settings 

    Args:
        env (SCons.Environment): Environment to modify
    """
    ostype = utils.ostype()
    cxx = env['CXX']
    btype = env['BUILD_TYPE']
    flist = ['general', 'warnings', btype.lower()]
    keys = _compiler_map.keys()
    cname = os.path.basename(cxx)
    for k in keys:
        if k == cname:
            cname = k
            break
    cflags = _compiler_flags_map[_compiler_map[cname]]
    # Compiler flags
    env.Append(CXXFLAGS = '-D'+ostype)
    env.Append(CXXFLAGS = '-DWM_'+env['PRECISION'])
    for k in flist:
        env.Append(CXXFLAGS = cflags[k].split())

    # Linker flags
    env.Append(LIBPATH_COMMON = [env['LIB_PLATFORM_INSTALL']],
               LIBPATH_APPS = [], LIBPATH_LIBS = [])
    env.Append(RPATH = [env['LIB_PLATFORM_INSTALL']])
    if not ostype == "windows":
        env.Append(LIBS = [env['MPI_LIB_NAME'], 'z'])
    # Global include directory
    env.Append(CPPPATH = [env['PROJECT_INC_DIR']])

    arch_func = _arch_map.get(ostype,None)
    if arch_func is not None:
        arch_func(env)

    # LEX replacement
    env.Replace(LEXCOM='$LEX $LEXFLAGS -o$TARGET -f $SOURCES')

    # Fix MPI flags
    mpi_lib = env['MPI_LIB_NAME']
    if "mpich" in mpi_lib.lower():
        env.Append(CXXFLAGS = '-DMPICH_SKIP_MPICXX')
    else:
        env.Append(CXXFLAGS = '-DOMPI_SKIP_MPICXX')
