# -*- coding: utf-8 -*-

"""\
Caelus SCons build environment
-------------------------------

Creates the base build environment used during the Caelus compilation process.
"""

import os
from collections import OrderedDict
import SCons.Script
from SCons.Environment import Environment
from variables import caelus_vars, init_dependent_vars
from utils import ostype

def caelus_env():
    """Create and return a default build environemnt"""
    tools = ['default']
    if ostype() == 'windows':
        tools = ['mingw']

    env = Environment(variables = caelus_vars,
                      tools = tools,
                      ENV = os.environ)
    init_dependent_vars(env)
    return env

def populate_env_vars(env):
    """Populate an ordered dict with environment variables"""
    def get_osvar(vname):
        """Return the appropriate variable format"""
        return "%%%s%%"%vname if ostype() == 'windows' else "$%s"%vname

    varnames = """project_name project_version project_dir
        build_option external_dir mpi_inc_path mpi_lib_path
        mpi_lib_name bin_platform_install lib_platform_install
        """.upper().split()

    env_vars = OrderedDict()
    for key in varnames:
        env_vars[key] = env[key]

    env_vars['PROJECT'] = env['PROJECT_NAME'] + "-" + env['PROJECT_VERSION']
    env_vars['PROJECT_VER'] = env['PROJECT_VERSION']
    env_vars['CAELUS_PROJECT_DIR'] = env['PROJECT_DIR']

    prj_dir = env['PROJECT_DIR']
    env_vars['LIB_SRC'] = os.path.join(prj_dir, 'src', 'libraries')
    env_vars['CAELUS_APP'] = os.path.join(prj_dir, 'applications')
    env_vars['CAELUS_SOLVERS'] = os.path.join(
        prj_dir, 'applications', 'solvers')
    env_vars['CAELUS_UTILITIES'] = os.path.join(
        prj_dir, 'applications', 'utilities')
    env_vars['CAELUS_TUTORIALS'] = os.path.join(prj_dir, 'tutorials')

    user_dir = env['USER_DIR']
    env_vars['CAELUS_USER_DIR'] = user_dir
    env_vars['CAELUS_USER_APPBIN'] = os.path.join(
        user_dir, 'platforms', env['BUILD_OPTION'], 'bin')
    env_vars['CAELUS_USER_LIBBIN'] = os.path.join(
        user_dir, 'platforms', env['BUILD_OPTION'], 'lib')

    if env['PROJECT_DIR'] in env['MPI_LIB_PATH']:
        env_vars['OPAL'] = os.path.dirname(env['MPI_LIB_PATH'])
        env_vars['OPAL_PREFIX'] = env_vars['OPAL']
        env_vars['PATH'] = (
            env['BIN_PLATFORM_INSTALL'] + os.pathsep +
            env_vars['CAELUS_USER_APPBIN'] + os.pathsep +
            os.path.normpath(
                os.path.join(env['MPI_LIB_PATH'], os.pardir, 'bin')) +
            os.pathsep + get_osvar('PATH'))
    else:
        env_vars['PATH'] = (
            env['BIN_PLATFORM_INSTALL'] + os.pathsep +
            env_vars['CAELUS_USER_APPBIN'] + os.pathsep +
            get_osvar('PATH'))

    if ostype() == 'windows':
        env_vars['PATH'] = (
            env['LIB_PLATFORM_INSTALL'] + os.pathsep +
            env_vars['CAELUS_USER_LIBBIN'] + os.pathsep +
            env['MPI_LIB_PATH'] + os.pathsep + env_vars['PATH'])
    else:
        ename = ('LD_LIBRARY_PATH' if ostype() != 'darwin'
                 else 'DYLD_FALLBACK_LIBRARY_PATH')
        env_vars[ename] = (
            env['LIB_PLATFORM_INSTALL'] + os.pathsep +
            env_vars['CAELUS_USER_LIBBIN'] + os.pathsep +
            env['MPI_LIB_PATH'] + os.pathsep +
            get_osvar(ename))

    env_vars['MPI_BUFFER_SIZE'] = "20000000"

    zlib_dir = env['ZLIB_PATH']
    if os.path.exists(zlib_dir):
        env_vars['ZLIB_PATH'] = zlib_dir

    env_vars['SCONSFLAGS'] = '--site-dir=%s'%os.path.dirname(__file__)

    return env_vars

def write_windows_env(env_vars, etc_dir):
    """Write out the windows environment file"""
    header = """
REM Caelus run environment
@echo off

"""
    cmd_fmt = '@set %s=%s\n'
    env_cmd_file = os.path.join(etc_dir, "caelus-environment.cmd")
    if os.path.exists(env_cmd_file):
        print ("Over-writing cmd environment file...")
    else:
        print ("Writing cmd environment file...")

    print ("Writing windows environment file...")
    with open(env_cmd_file, 'w') as fh:
        fh.write(header)
        for key, value in env_vars.items():
            fh.write(cmd_fmt%(key, value))

def write_unix_env(env_vars, etc_dir):
    """Write out unix environment files"""
    bash_fmt = 'export %s="%s"\n'
    csh_fmt = 'setenv %s "%s"\n'
    bashrc_file = os.path.join(etc_dir, "caelus-bashrc")
    cshrc_file = os.path.join(etc_dir, "caelus-cshrc")
    if os.path.exists(bashrc_file):
        print ("Over-writing unix bash and csh environment files...")
    else:
        print ("Writing unix bash and csh environment files...")

    with open(bashrc_file, 'w') as fh:
        fh.write("#!/bin/bash\n")
        fh.write("#\n# Bash configuration file for %s\n\n"%env_vars["PROJECT"])
        for key, value in env_vars.items():
            fh.write(bash_fmt%(key, value))
    with open(cshrc_file, 'w') as fh:
        fh.write("#!/bin/csh\n")
        fh.write("#\n# csh configuration file for %s\n\n"%env_vars["PROJECT"])
        for key, value in env_vars.items():
            fh.write(csh_fmt%(key, value))

    os.chmod(bashrc_file, 493)
    os.chmod(cshrc_file, 493)

def dump_environment(env):
    """Write out the environment"""
    site_scons_dir = os.path.dirname(__file__)
    etc_dir = os.path.normpath(
        os.path.join(site_scons_dir, os.pardir, "etc"))
    env_vars = populate_env_vars(env)
    if ostype() == 'windows':
        write_windows_env(env_vars, etc_dir)
    else:
        write_unix_env(env_vars, etc_dir)
