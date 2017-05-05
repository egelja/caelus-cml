Caelus SCons refactoring
========================

# Caelus SCons build system

The existing build system has been refactored to remove the dependency on Caelus
python module as well as the existence of environment variables. The only
dependency for the build system is SCons and the user can control the
compilation settings using a build configuration file or by providing parameters
on the command line as arguments to scons executable. 

Key changes:

- Follow SCons layout. All functionality is consolidated within modules provided
  in `site_scons` directory at the top-level. This directory is automatically
  sourced by `scons` during compilation process. 

- One `SConstruct` file to control compilation. Capability to detect dependency
  between Caelus dependencies, library changes that affect application
  executables. 
  
- Ability to do partial compiles by providing path to the relevant `SConscript`
  directory. Example:
  
  ```
  # Recompile RAS libraries only
  cd src/libraries/turbulenceModels/incompressible/RAS
  scons -u 
  
  # Example: Recompile pisoSolver only
  cd src/applications/solvers/incompressible/pisoSolver
  scons -u
  ```

- Use SCons configuration system to provide compilation settings. Use `scons -h`
  to see the available variables that control the compilation process.
  
- Capability to control the compile time settings either using a configuration
  file (`build_config.py`) at the top-level (in the same directory as
  `SConstruct`) or via command line. See `build_config_template.py` for an
  example. 
  
  ```
  # Example command line usage to enable SWAK
  scons BUILD_SWAK=True
  ```

- Separate build directory (`variantDir` in SCons parlance) to avoid cluttering
  the source directories with object files and executables. Cleaner `git status`
  output that does not contain untracked file information. The build directory
  is segregated based on the compiler, precision, and optimization settings. 
  
- Handle generation and cleanup of `lnInclude` directories using built-in SCons
  modules. This has the benefit of using SCons dependency tracking mechanism to
  automatically generate and cleanup `lnInclude`. This solves the issue where
  previously the `lnInclude` directory was created even when using `scons -h` or
  scons dry-run options. With the new approach, dry-run will not create
  `lnInclude` directories. 

# Caelus SCons organization

## `site_scons` directory

All the logic necessary for compiling Caelus is coded up in modules within the
`site_scons` directory. The main files are described below:

- `variables.py` : This file defines the configuration variables available for
  controlling the compilaton process. The user-configuration variables are
  defined using SCons `Variables` class. This provides automatic help message
  generation and checking the user provided values for consistency. Once the
  configuration file and command line settings are processed, this file contains
  the logic to generate dependent variables based on user provided config. See
  top-level `Environment` creation in `SConstruct` file to see how this is used.
  The developers can control the core settings during release process by
  updating the variables defined in this file. 
  
- `compiler.py`: This module contains the logic to control the compiler
  selection, compilation flags, etc. The function `update_compiler_settings` is
  invoked immediately after `Environment` creation to set up Caelus compilation.
  
- `build.py`: This module defines two main functions: `build_app` and
  `build_lib` that handle the boilerplate code necessary for building an
  executable and a shared library respectively. See example usage in
  `SConscript` files within the `src/libraries` and `src/applications`
  directories. 
  
- `utils.py`: Miscellaneous utilities used in other `site_scons` modules as well
  as in `SConstruct` and `SConscript` files. 
  
## `SConstruct` file

There is one master `SConstruct` file for the entire Caelus compilation process.
The compilation process can be kicked off within any subdirectory within Caelus
using the `scons -u` flag. This will only compile the directory (and its
subdirectories) from which the command is executed. This file sets up the
`variantDir` where the object files generated from the compilation process is
stored. 

## `SConscript` files

There are two kinds of `SConscript` files:

The top-level `SConscript` files (e.g., `src/libraries/SConscript`) just
contains a list of target directories and loops over the `SConscript` files
found in these targets.

The target `SConscript` files contain the build definitions for building a
library or an executable. They mostly import `build_lib` or `build_app` from
`site_scons/build.py` and invoke these functions with appropriate configuration.
Typically, these files should contain the following variables.

- `target` : The name of the target executable or library to be compiled. 

- `sources` : A list of source files on which the target depends. 

- `caelus_inc`: A list of include directories (usually `lnInclude`) that needs
  to be passed to the compiler. The `build` function automatically adds the
  `src/libraries` prefix, so it is only required to pass the path with respect
  to `src/libraries`.
  
- `caelus_libs`: A list of libraries to link to. 

In addition to the aforementioned common variables, the user can provide two
additional variables to control other settings: `prepend_args` and
`append_args`. Both these variables are dictionaries that contain (key, value)
pairs that are used to change the `Environment`. As the name indicates, the
values provided are added to the environment using `Prepend` and `Append`
commands. See `src/libraries/parallel/decompose/ptscotchDecomp/SConscript` for
an example usage.

`
prepend_flags = {
   "CXXFLAGS" : ['-O3'],

   "CPPPATH" : ["my_non_standard_include_path"],

   "LIBPATH" : ["my_non_standard_lib_path"],

   "LIBS" : ["my_custom_libs"]
}
`

# Compilation instructions

- Install `scons` either using `pip` or `conda`. It is highly recommended to
  isolate it within a `virtualenv`. 
  
- Run `scons -h` to examine the compilation variables available and determine if
  the default settings are acceptable. (Tip: use `scons -H` to see `scons` usage
  help.)
  
- Create a `build_config.py` (see template below) with settings appropriate for
  your system. 
  
- Invoke `scons install` at the top-level directory to kickoff the compilation
  process. To speed up the compilation use `-j <N>` option to enable parallel
  compilation, where `<N>` is the number of parallel jobs to start. 
  
- Once compilation is completed, the executables should be available in
  `platforms/<BUILD_TYPE>/bin` directory. 
  
## Sample `build_config.py` file 

E.g. for ubuntu systems:

```
# -*- coding: utf-8 -*-

"""Caelus configuration file for Ubuntu LTS 14.04

Ensure that you have installed OpenMPI and other dependencies via
apt-get.

sudo apt-get install libopenmpi-dev libcgal-dev libptscotch-dev libscotch-dev

"""

CC = "gcc"
CXX = "g++"
BUILD_TYPE = "Opt"
BUILD_ARCH = "64"
PRECISION = "DP"
MPI_LIB_NAME = "mpi"
MPI_INC_PATH = "/usr/include/openmpi/"
MPI_LIB_PATH = "/usr/lib/openmpi/"

```

