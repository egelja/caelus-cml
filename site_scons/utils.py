# -*- coding: utf-8 -*-

"""\
Caelus SCons build utilities
----------------------------

Collection of miscellaneous utilities that don't fit elsewhere. 
"""

import os
import platform

def ostype():
    """Return the operating system type"""
    if os.name == 'nt':
        return 'windows'
    else:
        return os.uname()[0].lower()


def ensure_metis(env):
    """Ensure that the environment is setup for METIS

    If ``BUILD_METIS`` is set to False, then the code expects the following
    variables to be setup:

    - METIS_LIB_PATH: Path to metis library files
    - METIS_INC_PATH: Path to metis include files

    - METIS_PATH: Path to METIS files; automatically sets the include and lib
      paths.

    """
    if env['BUILD_METIS']:
        print ("METIS will be built; ignoring request to check METIS")
        return

    metis_path = env['METIS_PATH']
    lib_path = env['METIS_LIB_PATH']
    inc_path = env['METIS_INC_PATH']

    if lib_path:
        assert os.path.exists(lib_path), "Cannot find METIS libraries. Check METIS_LIB_PATH settings"
        assert os.path.exists(inc_path), "Cannot find METIS headers. Check METIS_INC_PATH"
    else:
        assert os.path.exists(metis_path), "Cannot find METIS. Turn BUILD_METIS to True or provide paths to a valid METIS installation"
        env.Replace('METIS_INC_PATH',
                    os.path.join(metis_path,'include'))
        env.Replace('METIS_LIB_PATH',
                    os.path.join(metis_path,'lib'))

def ensure_scotch(env):
    """Ensure that the environment is setup for SCOTCH

    If ``BUILD_SCOTCH`` is set to False, then the code expects the following
    variables to be setup:

    - SCOTCH_LIB_PATH: Path to scotch library files
    - SCOTCH_INC_PATH: Path to scotch include files

    - SCOTCH_PATH: Path to SCOTCH files; automatically sets the include and lib
      paths.

    """
    if env['BUILD_SCOTCH']:
        print ("SCOTCH will be built; ignoring request to check SCOTCH")
        return

    scotch_path = env['SCOTCH_PATH']
    lib_path = env['SCOTCH_LIB_PATH']
    inc_path = env['SCOTCH_INC_PATH']

    if lib_path:
        assert os.path.exists(lib_path), "Cannot find SCOTCH libraries. Check SCOTCH_LIB_PATH settings"
        assert os.path.exists(inc_path), "Cannot find SCOTCH headers. Check SCOTCH_INC_PATH"
    else:
        assert os.path.exists(scotch_path), "Cannot find SCOTCH. Turn BUILD_SCOTCH to True or provide paths to a valid SCOTCH installation"
        env.Replace('SCOTCH_INC_PATH',
                    os.path.join(scotch_path,'include'))
        env.Replace('SCOTCH_LIB_PATH',
                    os.path.join(scotch_path,'lib'))


def ensure_gsl(env):
    """Ensure that the environment is setup for GSL

    If ``BUILD_GSL`` is set to False, then the code expects the following
    variables to be setup:

    - GSL_LIB_PATH: Path to gsl library files
    - GSL_INC_PATH: Path to gsl include files

    - GSL_PATH: Path to GSL files; automatically sets the include and lib
      paths.

    """
    if env['BUILD_GSL']:
        print ("GSL will be built; ignoring request to check GSL")
        return

    gsl_path = env['GSL_PATH']
    lib_path = env['GSL_LIB_PATH']
    inc_path = env['GSL_INC_PATH']

    if lib_path:
        assert os.path.exists(lib_path), "Cannot find GSL libraries. Check GSL_LIB_PATH settings"
        assert os.path.exists(inc_path), "Cannot find GSL headers. Check GSL_INC_PATH"
    else:
        assert os.path.exists(gsl_path), "Cannot find GSL. Turn BUILD_GSL to True or provide paths to a valid GSL installation"
        env.Replace('GSL_INC_PATH',
                    os.path.join(metis_path,'include'))
        env.Replace('GSL_LIB_PATH',
                    os.path.join(metis_path,'lib'))


def get_git_version(branch='HEAD'):
    """Return the SHA ID of the latest commit for the given branch

    Returns:
        str: SHA ID for the git commit or empty string 
    """
    import shlex
    from subprocess import Popen, PIPE
    shell = True if ostype() == "windows" else False
    cmdline = shlex.split("git rev-parse %s"%branch)
    cmd = Popen(cmdline, stdout=PIPE, stderr=PIPE, shell=shell)
    stdout, stderr = cmd.communicate()
    if stderr:
        # Encountered some error, so return empty string
        return ''
    else:
        return stdout.strip()

def tag_build_version(target, source, env):
    """Tag the build version for dependency tracking"""
    cver = env['PROJECT_VERSION']
    gitver = get_git_version()
    if gitver:
        build_str = "%s-%s"%(cver, gitver)
    else:
        build_str = cver
    output = True
    if os.path.exists(target[0].abspath):
        old_stamp = open(target[0].abspath).read().strip()
        if old_stamp == build_str:
            output = False
    if output:
        with open(target[0].abspath,'w') as fh:
            fh.write(build_str)

def stamp_caelus_version(target, source, env):
    """Generate Caelus global.cpp file"""
    cver = env['PROJECT_VERSION']
    gitver = get_git_version()
    if gitver:
        build_str = "%s-%s"%(cver, gitver[:12])
    else:
        build_str = cver
    data = open(source[0].abspath).read()
    data_out = data.replace(
        'VERSION_STRING', cver).replace(
            'BUILD_STRING',build_str)
    with open(target[0].abspath, 'w') as fh:
        fh.write(data_out)

