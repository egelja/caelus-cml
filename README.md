# Caelus

## License

Caelus is a derivative of OpenFOAM and contains modified source code from the OpenFOAM Foundation <https://www.openfoam.org>, OpenCFD <https://www.openfoam.com>, and foam-extend <https://sourceforge.net/projects/foam-extend/>. The mesh and field data formats are compatible with OpenFOAM.

It is released under the GPL: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

## Contributors
see CONTRIBUTORS.md

## COPYRIGHT Applied CCM 2014-2018

Current version: Caelus 9.04

## Solvers
### basic:
- **`potenialSolver`**: potential flow solver provided by the OpenFOAM Foundation
### incompressible:
- **`simpleSolver`**: SIMPLE-based incompressible steady-state solver
- **`SRFSimpleSolver`**: SIMPLE-based incompressible steady-state solver in a single rotating rotating reference frame
- **`pimpleSolver`**: PIMPLE-based incompressible transient solver with static and dynamic meshes
- **`SRFPimpleSolver`**: PIMPLE-based incompressible transient solver in a single rotating reference frame
### compressible:
- **`explicitDBNSSolver`**: density-based explicit compressible solver
- **`explicitDBNSSRFSolver`**: density-based explicit compressible solver in single rotating reference frame using the absolute velocity formulation
- **`cpbnsLTS`**: steady-state pressure-based compressible solver
- **`cpbnsPIMPLE`**: transient pressure-based compressible solver
### heat-transfer:
- **`buoyantBoussinesqSimpleSolver`**: SIMPLE-based incompressible steady-state solver with buoyant body-force based on Boussinesq approximation
- **`buoyantSimpleSolver`**: SIMPLE-based incompressible steady-state solver with buoyant body-force
### multiphase:
- **`vofSolver`**: Volume of Fluid (VOF) phase-fraction based solver for two incompressible, isothermal immiscible fluids with static
- **`porousWaveSolver`**: `vofSolver` with **waves2Foam** functionality enabled (see below)
- **`isoVofSolver`**: Volume of Fluid (VOF) phase-fraction based solver for two incompressible, isothermal immiscible fluids using the iso-advector phase-fraction based interface capturing approach of Roenby et al.
- **`vofLPTSolver`**: Volume of Fluid (VOF) phase-fraction based solver with Lagrangian particle tracking
- **`pimpleParcelSolver`**: PIMPLE-based incompressible transient solver with Lagrangian particle tracking

### combustion:
- **`reactingSolver`**: A fractional step based transient solver for compressible combustion flows with the Arrhenius rate chemistry ODE solver. Time stepping algorithm is based on PIMPLE algorithm
## Turbulence Models
### Reynolds Avergaged Navier-Stokes:
- Spalart-Allmaras with curvature correction
- Realizable k-epsilon
- k-epsilon
- k-omega SST with curvature correction (Menter et al 2003)
- Scale Adapted Simulation (SAS) based on k-omega SST (Menter er al 2003)
### Transition:
- gamma-Re-Theta; calibrated for commercial CFD (Malan et al 2009)
#### Wall Functions:
- nutU high Reynolds number velocity based
- nutUSpalding all Reynolds number velocity based
### Very Large Eddy Simulation:
- Spalart-Allmaras with curvature correction
- k-omega SST with curvature correction
- Realizable k-epsilon
- coherent structure with curvature correction
### Detached Eddy Simulation:
- Spalart-Allmaras Delayed DES
- Spalart-Allmaras Improved Delayed DES
- k-omega SST Delayed DES
### Large Eddy Simulation:
- Smagorinsky
- dynamic sub-grid scale kinetic energy
- coherent structure
- WALE
#### Wall Damping:
- van Driest

## *waves* library
The *waves* library is a port of Niels Gjoel Jacobsen's waves2Foam to Caelus. It is a suite of wave generation and absorption solvers and libraries. More information about waves2Foam is available at:

<http://openfoamwiki.net/index.php/Contrib/waves2Foam>

This functionality is enabled through the `porousWaveSolver`. Refer to the `waves` tutorial cases for usage examples of the waves2Foam utilities and solvers.

## Installation
Installers available for download at <http://www.caelus-cml.com/download/>.

### Windows
- The Windows installer is not digitally signed and may cause some browsers to flag the installer as containing a virus. If this occurs, accept the download or try a different browser.
- For Windows 8, the MPICH2 software requires .NET 2.050727 to be installed. This is provided in the prerequisites package.
- For MPICH2 installation see below.
- To install Caelus, double click the .exe and follow the on screen prompts.
- There are two ways to setup the Caelus environment:
    * If you use the Caelus Python Library (CPL) to run Caelus you do not need to do anything further to set up Caelus other than double clicking the icon on the desktop.
    * If you to want use the previous method to run Caelus then execute ‘call {Caelus install path}\etc\caelus-environment.cmd’ from a command prompt to configure the environment.
    
#### MPICH2 on Windows
- During the installation select MPICH2 to be installed for Everyone
- After installing MPICH2, you may need to add rules to your firewall to allow the mpiexec.exe and smpd.exe programs to communicate through the firewall. This process depends on your firewall configuration.
- After installing Caelus, Open an admin command prompt by right-clicking on the command prompt icon and selecting “Run as Administrator“. In the command prompt type the following sequence of commands:
    * smpd -install
    * mpiexec -remove
    * mpiexec -register (note you should use your username and password as user and not administrator)
    * mpiexec -validate (should return success)
    * smpd -status (should return ‘smpd running on {hostname}’)

### Linux
- For Ubuntu 14.04/16.04 the following prerequisites are required (install using apt-get):
    * build-essential
    * flex
    * bison
    * zlib1g-dev
- For RHEL 6.8/7 the following prerequisites are required (install using yum):
    * groupinstall “Development Tools”
- To install Caelus, change installer permissions to executable (chmod +x) and execute the .run package.
- There are two ways to setup the Caelus environment:
    * If you use the CPL to run Caelus you do not need to do anything further to set up Caelus. The installer adds the CPL exe path to the system PATH
    * If you to want use the previous method to run Caelus then type
    ```
     source etc/caelus-bashrc
    ```
from a command prompt to configure the run environment. For users of the C shell, the caelus-cshrc from the etc directory is provided. For users of other shells, use the existing shell files as a template to create an appropriate environment file.
- The installer can be run in text mode by issuing the following the installation command with “–mode text” (without quotes).

### Mac OS X
- To run the pre-built binaries of Caelus no prerequisites are required.
- Run the .app file don’t drag it into the Application folder.
- To compile Caelus, the clang compiler is required and is included with the Apple developer tools package. To easiest method to install the developer tools package is to:
    * start a Terminal
    * type ‘clang’ (without quotes)
    * click ‘Install’ in the pop-up dialogue to install the developer tools
    * Note: Xcode is not necessary to compile Caelus
- There are two ways to setup the Caelus environment:
    * If you use the CPL to run Caelus you do not need to do anything further to set up Caelus. The installer adds the CPL exe path to the system PATH
    * If you to want use the previous method to run Caelus then type
    ```
     source etc/caelus-bashrc
    ```
from a command prompt to configure the run environment. For users of the C shell, the caelus-cshrc from the etc directory is provided. For users of other shells, use the existing shell files as a template to create an appropriate environment file.
- The installer can be run in text mode by issuing the following the installation command with “–mode text” (without quotes).
    
## Caelus Python Library
The Caelus Python Library (CPL) provides a mechanism that does not require sourcing or calling an environment file to run applications. The CPL documentation can be found at <http://caelus.readthedocs.io>. CPL is included in the Caelus installers or can be obtained from the public repository.

### Usage
Running an application:

_previous_
```Bash
 $> mpiexec -n 4 simpleSolver -parallel &> simpleSolver.log &
```
_new_:
```Bash
 $> caelus run -p simpleSolver
```
Type `caelus -h` for the complete list of subcommands, `caelus <subcommand> -h` for the subcommand's options, or refer to documentation.

### Workflows
With CPL, workflows are straightforward to setup with YAML-based task files. For example, the the `pitzDailyExptInlet` tutorial the YAML-based `run_tutorial.yaml` task file:

```Yaml
tasks:
  - clean_case:
      remove_zero: no
      remove_mesh: yes

  - run_command:
      cmd_name: blockMesh

  - run_command:
      cmd_name: decomposePar
      cmd_args "-force"

  - run_command:
      cmd_name: simpleSolver
      parallel: True

  - run_command:
      cmd_name: reconstructPar
      cmd_args "-latestTime"

  - process_logs:
      log_file: simpleSolver.log
```
Task files are run by:

```Bash
 $> caelus tasks -f run_tutorial.py
```

### Build
CPL now provides an interface to compile Caelus and external Caelus-based code.

```Bash
 $> caelus build
```
Parallel compilation can be invoked with the -j [nProcs] option.

By default a log of the build process is sent to `~/.caelus/cml_build_logs/cml_build.log` but can be changed with the `-l [log file name]` option.

### Parametric Runs
CPL provides a parametric run capability, accessible from the command-line via the `caelus_sim` application. To use the parametric run capability, a simulation configuration file (in YAML format) and a template case directory are required. Refer to the [parametric run](https://caelus.readthedocs.io/en/latest/user/tutorials/caelus_sim.html) tutorial for a full explanation.

## Contributors Repository Set Up
There is a public git repository for the Caelus source code that tracks with the current release

https://bitbucket.org/appliedccm/caelus-cml.

To make a local copy, clone the repository with git:

```Bash
 $> git clone git@bitbucket.org:appliedccm/caelus-cml.git
```
Create and modify a `build_config.py` in the top-level directory where the Caelus repository was cloned. A basic build_config.py on Linux, which uses a system MPI, would take the following form:

```Python
"""
Caelus configuration file
"""

PROJECT_NAME = "Caelus"
PROJECT_VERSION = "9.04"

CC = "gcc"
CXX = "g++"
OMP = True
BUILD_TYPE = "Opt"
BUILD_ARCH = "64"
PRECISION = "DP"

MPI_LIB_NAME = 'mpi'
MPI_LIB_PATH = '/usr/lib/openmpi/lib'
MPI_INC_PATH = '/usr/lib/openmpi/include
```
To compile Caelus, in the Caelus repository directory, type:

**new**:
```Bash
 $> caelus build
```
Parallel compilation can be invoked with the `-j [nProcs]` option.

Upon successful compilation, run Caelus using CPL. To compile applications and libraries outside the Caelus repository directory use `caelus build`.

**previous**:
First, ensure the SCons directory is in the system PATH, `${CAELUS_PROJECT_DIR}/external/scons-local-3.0.3`.

```Bash
 $> scons.py install
```
Parallel compilation can be invoked with the `-j [nProcs]` option.

Upon successful compilation, run Caelus per the above platform-specific instructions. To compile applications and libraries outside the Caelus repository directory, provide scons with the location of the site_scons directory. For example,

```Bash
 $> scons.py --site-dir="/home/username/Caelus/Caelus-9.04/site_scons" install
```
Run `scons.py -h` to see the full list of build options or refer to the scons_notes.md for further details about the build system.

## CPL Repository Setup
Refer to the CPL [installation](http://caelus.readthedocs.io/en/latest/user/installation.html) and [configuration](http://caelus.readthedocs.io/en/latest/user/configuration.html) documentation.
