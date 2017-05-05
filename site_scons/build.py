# -*- coding: utf-8 -*-

"""\
Caleus SCons build rules
"""

import os
import utils

def build_lib(baseenv, target, sources,
              caelus_inc, caelus_libs,
              prepend_args=None,
              append_args=None,
              lib_type="shared"):
    """Build a shared library

    Args:
        baseenv (env): Caelus SCons build environment
        target (str): Name of the build target
        sources (list): List of sources for this target
        caelus_inc (list): List of Caelus include paths
        caelus_libs (list): List of libraries to be linked

        prepend_args (dict): Set of (key, value) pairs to be prepended
        append_args (dict): Set of (key, value) pairs to be appended
    """
    libenv = baseenv.Clone()

    lib_src = libenv['LIB_SRC']
    inc_dirs = [os.path.join(lib_src,d) for d in caelus_inc]
    libenv.Prepend(CPPPATH = 
            inc_dirs +
            [os.path.join(libenv.Dir(".").srcnode().abspath,
                                           "lnInclude")])
    libenv.Append(LIBS = caelus_libs)
    libenv.Append(LIBPATH =
                  libenv['LIBPATH_COMMON'] + libenv['LIBPATH_LIBS'])

    if utils.ostype() == "darwin":
       exec_path = '-install_name @executable_path/../lib/lib{0}.dylib'.format(target)
       libenv.Append(SHLINKFLAGS = exec_path)

    if lib_type == "shared":
        exe = libenv.SharedLibrary(target=target, source=sources)
    elif lib_type == "static":
        exe = libenv.StaticLibrary(target=target, source=sources)
    elif lib_type == "object":
        exe = libenv.Object(target=target, source=sources)
    install_dir = libenv['LIB_PLATFORM_INSTALL']
    libenv.Alias('install', install_dir)
    libenv.Install(install_dir, exe)

    if prepend_args is not None:
        libenv.Prepend(**prepend_args)
    if append_args is not None:
        libenv.Append(**append_args)

    return libenv

def build_app(baseenv, target, sources,
              caelus_inc, caelus_libs,
              prepend_args=None,
              append_args=None):
    """Build an executable application

    Args:
        baseenv (env): Caelus SCons build environment
        target (str): Name of the build target
        sources (list): List of sources for this target
        caelus_inc (list): List of Caelus include paths
        caelus_libs (list): List of libraries to be linked

        prepend_args (dict): Set of (key, value) pairs to be prepended
        append_args (dict): Set of (key, value) pairs to be appended
    """
    appenv = baseenv.Clone()

    lib_src = appenv['LIB_SRC']
    inc_dirs = [os.path.join(lib_src,d) for d in caelus_inc]
    appenv.Prepend(CPPPATH = inc_dirs)
    appenv.Append(LIBS = caelus_libs)
    appenv.Append(LIBPATH =
                  appenv['LIBPATH_COMMON'] + appenv['LIBPATH_APPS'])

    exe = appenv.Program(target=target, source=sources)
    install_dir = appenv['BIN_PLATFORM_INSTALL']
    appenv.Alias('install', install_dir)
    appenv.Install(install_dir, exe)

    if prepend_args is not None:
        appenv.Prepend(**prepend_args)
    if append_args is not None:
        appenv.Append(**append_args)

    return appenv

def process_lninclude(env, basedir,
                      symlink=False):
    """Create lnInclude directories

    Args:
        env (Environment): Caelus SCons build environment
        basedir (path): Directory to traverse
    """
    from SCons.Script import Copy, Mkdir, Dir

    ostype = "windows" if utils.ostype() == "windows" else "posix"
    include_patterns = [".hpp", ".H", ".hxx", ".h", ".hh"]
    bdir = Dir(basedir).srcnode().abspath
    inc_dir = os.path.join(bdir, "lnInclude")
    Mkdir(inc_dir)

    inc_env = env.Clone()
    if os.path.exists(inc_dir):
        inc_env.Alias("install", inc_dir)

    for root, dlist, files in os.walk(bdir):
        if "lnInclude" in dlist:
            dlist.remove("lnInclude")

        dbase = os.path.basename(root)
        if "OSspecific" in dbase:
            for d in dlist:
                if d != ostype:
                    dlist.remove(d)

        for f in files:
            if any(f.endswith(pat) for pat in include_patterns):
                src = os.path.join(root, f)
                dest = os.path.join(inc_dir, f)
                yield inc_env.Install(inc_dir, src)
