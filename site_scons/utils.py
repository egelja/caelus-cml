# -*- coding: utf-8 -*-

"""\
Caelus SCons build utilities
----------------------------

Collection of miscellaneous utilities that don't fit elsewhere. 
"""

import os
import platform
import SCons.compat
import SCons.Subst
import tempfile

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
        return stdout.decode('utf-8').strip()

def get_git_branch():
    """Return the current branch

    Returns:
        str: Name of the current branch 
    """
    import shlex
    from subprocess import Popen, PIPE
    shell = True if ostype() == "windows" else False
    cmdline = shlex.split("git rev-parse --abbrev-ref HEAD")
    cmd = Popen(cmdline, stdout=PIPE, stderr=PIPE, shell=shell)
    stdout, stderr = cmd.communicate()
    if stderr:
        # Encountered some error, so return empty string
        return ''
    else:
        return stdout.decode('utf-8').strip()

def get_git_tag():
    """Return the closest annotated tag

    Returns:
        str: Name of the closest annotated tag 
    """
    import shlex
    from subprocess import Popen, PIPE
    shell = True if ostype() == "windows" else False
    cmdline = shlex.split("git describe --tags --abbrev=0")
    cmd = Popen(cmdline, stdout=PIPE, stderr=PIPE, shell=shell)
    stdout, stderr = cmd.communicate()
    if stderr:
        # Encountered some error, so return empty string
        return ''
    else:
        return stdout.decode('utf-8').strip()

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

def tag_user_build_version(target, source, env):
    """Tag the build version for tracking of the users library/application"""
    tag    = get_git_tag()
    branch = get_git_branch()
    gitver = get_git_version()
    if gitver:
        build_str = "%s-%s-%s"%(branch, tag, gitver)
    else:
        build_str = branch
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
    tag    = get_git_tag()
    if tag:
        tag_str = "%s"%(tag)
    else:
        tag_str = 'NOTAG'

    if gitver:
        build_str = "%s-%s"%(cver, gitver[:12])
    else:
        build_str = cver
    data = open(source[0].abspath).read()
    data_out = data.replace(
        'VERSION_STRING', cver).replace(
            'BUILD_STRING',build_str).replace(
            'TAG_STRING',tag_str)

    with open(target[0].abspath, 'w') as fh:
        fh.write(data_out)

def stamp_user_version(target, source, env):
    """Generate user version.hpp file"""
    tag    = get_git_tag()
    branch = get_git_branch()
    gitver = get_git_version()

    if tag:
        tag_str = "%s"%(tag)
    else:
        tag_str = 'NOTAG'

    if gitver:
        build_str = "%s-%s-%s"%(branch, tag, gitver[:12])
    else:
        build_str = branch
    data = open(source[0].abspath).read()
    data_out = data.replace(
        'BRANCH_STRING', branch).replace(
            'TAG_STRING', tag_str).replace(
            'BUILD_STRING', build_str)
    with open(target[0].abspath, 'w') as fh:
        fh.write(data_out)

class TempFileMunge_caelus(object):
    """A callable class.  You can set an Environment variable to this,
    then call it with a string argument, then it will perform temporary
    file substitution on it.  This is used to circumvent the long command
    line limitation.

    Example usage:
    env["TEMPFILE"] = TempFileMunge_caelus
    env["LINKCOM"] = "${TEMPFILE('$LINK $TARGET $SOURCES','$LINKCOMSTR')}"

    By default, the name of the temporary file used begins with a
    prefix of '@'.  This may be configred for other tool chains by
    setting '$TEMPFILEPREFIX'.

    env["TEMPFILEPREFIX"] = '-@'        # diab compiler
    env["TEMPFILEPREFIX"] = '-via'      # arm tool chain
    """

    def __init__(self, cmd, cmdstr = None):
        self.cmd = cmd
        self.cmdstr = cmdstr

    def __call__(self, target, source, env, for_signature):
        if for_signature:
            # If we're being called for signature calculation, it's
            # because we're being called by the string expansion in
            # Subst.py, which has the logic to strip any $( $) that
            # may be in the command line we squirreled away.  So we
            # just return the raw command line and let the upper
            # string substitution layers do their thing.
            return self.cmd

        # Now we're actually being called because someone is actually
        # going to try to execute the command, so we have to do our
        # own expansion.
        cmd = env.subst_list(self.cmd, SCons.Subst.SUBST_CMD, target, source)[0]
        try:
            maxline = int(env.subst('$MAXLINELENGTH'))
        except ValueError:
            maxline = 2048

        length = 0
        for c in cmd:
            length += len(c)
        length += len(cmd) - 1
        if length <= maxline:
            return self.cmd

        # Check if we already created the temporary file for this target
        # It should have been previously done by Action.strfunction() call
        node = target[0] if SCons.Util.is_List(target) else target
        cmdlist = getattr(node.attributes, 'tempfile_cmdlist', None) \
                    if node is not None else None
        if cmdlist is not None :
            return cmdlist

        # We do a normpath because mktemp() has what appears to be
        # a bug in Windows that will use a forward slash as a path
        # delimiter.  Windows's link mistakes that for a command line
        # switch and barfs.
        #
        # We use the .lnk suffix for the benefit of the Phar Lap
        # linkloc linker, which likes to append an .lnk suffix if
        # none is given.
        (fd, tmp) = tempfile.mkstemp('.lnk', text=True)
        native_tmp = SCons.Util.get_native_path(os.path.normpath(tmp))

        if env.get('SHELL',None) == 'sh':
            # The sh shell will try to escape the backslashes in the
            # path, so unescape them.
            native_tmp = native_tmp.replace('\\', r'\\\\')
            # In Cygwin, we want to use rm to delete the temporary
            # file, because del does not exist in the sh shell.
            rm = env.Detect('rm') or 'del'
        else:
            # Don't use 'rm' if the shell is not sh, because rm won't
            # work with the Windows shells (cmd.exe or command.com) or
            # Windows path names.
            rm = 'del'

        prefix = env.subst('$TEMPFILEPREFIX')
        if not prefix:
            prefix = '@'

        args = list(map(SCons.Subst.quote_spaces, cmd[1:]))
        output = " ".join(args).replace("\\", "\\\\") 
        os.write(fd, bytearray(output + "\n",'utf-8'))
        #os.write(fd, bytearray(" ".join(args) + "\n",'utf-8'))
        os.close(fd)
        # XXX Using the SCons.Action.print_actions value directly
        # like this is bogus, but expedient.  This class should
        # really be rewritten as an Action that defines the
        # __call__() and strfunction() methods and lets the
        # normal action-execution logic handle whether or not to
        # print/execute the action.  The problem, though, is all
        # of that is decided before we execute this method as
        # part of expanding the $TEMPFILE construction variable.
        # Consequently, refactoring this will have to wait until
        # we get more flexible with allowing Actions to exist
        # independently and get strung together arbitrarily like
        # Ant tasks.  In the meantime, it's going to be more
        # user-friendly to not let obsession with architectural
        # purity get in the way of just being helpful, so we'll
        # reach into SCons.Action directly.
        if SCons.Action.print_actions:
            cmdstr = env.subst(self.cmdstr, SCons.Subst.SUBST_RAW, target,
                               source) if self.cmdstr is not None else ''
            # Print our message only if XXXCOMSTR returns an empty string
            if len(cmdstr) == 0 :
                print("Using tempfile "+native_tmp+" for command line:\n"+
                      str(cmd[0]) + " " + " ".join(args))

        # Store the temporary file command list into the target Node.attributes
        # to avoid creating two temporary files one for print and one for execute.
        cmdlist = [ cmd[0], prefix + native_tmp + '\n' + rm, native_tmp ]
        if node is not None:
            try :
                setattr(node.attributes, 'tempfile_cmdlist', cmdlist)
            except AttributeError:
                pass
        return cmdlist
