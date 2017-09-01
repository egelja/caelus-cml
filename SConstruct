# -*- mode: python -*-

import os
import utils
from variables import caelus_vars, init_dependent_vars
from compiler import update_compiler_settings

### Initialize toolsets based on operating system
ostype = utils.ostype()
tools = ['default']
if ostype == 'windows':
    tools += ['mingw']

### Base SCons environment
env = Environment(variables = caelus_vars,
                  tools = tools,
                  ENV = os.environ)
Help(caelus_vars.GenerateHelpText(env))
init_dependent_vars(env)
update_compiler_settings(env)

### Isolate build environments based on build options
build_dir = os.path.join(
    Dir("#").abspath,"build",env['BUILD_OPTION'])

### Build dependencies first
if env['BUILD_METIS']:
    from variables import METIS_VERSION
    SConscript('external/metis-%s/SConscript'%METIS_VERSION,
               exports=['env'],
               src_dir=Dir("#").srcnode().abspath,
               variant_dir=build_dir)
else:
    utils.ensure_metis(env)

if env['BUILD_SCOTCH']:
    from variables import SCOTCH_VERSION
    SConscript('external/scotch-%s/SConscript'%SCOTCH_VERSION,
               exports=['env'],
               src_dir=Dir("#").srcnode().abspath,
               variant_dir=build_dir)
else:
    utils.ensure_scotch(env)

if env['BUILD_GSL']:
    from variables import GSL_VERSION
    SConscript('external/pgsl-%s/SConscript'%GSL_VERSION,
               exports=['env'],
               src_dir=Dir("#").srcnode().abspath,
               variant_dir=build_dir)
else:
    utils.ensure_gsl(env)

### Main Caelus entry points
caelus_src = Split("""
    src/libraries
    src/applications/solvers
    src/applications/utilities
""")

for d in caelus_src:
    SConscript('%s/SConscript'%d,
               exports=['env'],
               src_dir=Dir("#").srcnode().abspath,
               variant_dir=build_dir)

### Build optional features
if env['BUILD_SWAK']:
    swak_src = 'external/swak'
    SConscript(swak_src + '/SConscript',
               exports=['env'],
               src_dir=Dir("#").srcnode().abspath,
               variant_dir=build_dir)

if env['BUILD_CFMESH']:
    cfmesh_src = 'external/cfmesh'
    SConscript(cfmesh_src + '/SConscript',
               exports=['env'],
               src_dir=Dir("#").srcnode().abspath,
               variant_dir=build_dir)

### Remove buid directory when cleaning
Clean(".", build_dir)
