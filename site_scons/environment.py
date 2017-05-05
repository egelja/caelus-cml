# -*- coding: utf-8 -*-

"""\
Caelus SCons build environment
-------------------------------

Creates the base build environment used during the Caelus compilation process.
"""

import os
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
