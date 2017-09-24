# Caelus

## License

Caelus was forked from OpenFOAM and contains modified OpenFOAM source code. It is currently compatible with OpenFOAM.

It is released under the GPL: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

## Contact
- Aleks Jemcov: a.jemcov@appliedccm.com
- Chris Sideroff: c.sideroff@appliedccm.ca
- Darrin Stephens: d.stephens@appliedccm.com.au

## COPYRIGHT Applied CCM 2014-2017

Current version: Caelus 7.04

## Solvers
### basic:
- **`potenialSolver`**: potential flow solver provided by the OpenFOAM Foundation
### incompressible:
- **`simpleSolver`**: SIMPLE-based incompressible steady-state solver
- **`pCorrSimpleSolver`**: SIMPLE-based incompressible steady-state solver using pressure correction approach
- **`SRFSimpleSolver`**: SIMPLE-based incompressible steady-state solver in a single rotating rotating reference frame
- **`pimpleSolver`**: PIMPLE-based incompressible transient solver provided
- **`pimpleDyMSolver`**: PIMPLE-based incompressible transient dynamic mesh solver
- **`SRFPimpleSolver`**: PIMPLE-based incompressible transient solver in a single rotating reference frame
- **`pisoSolver`**: PISO-based incompressible transient solver
### compressible:
- **`explicitDBNSSolver`**: density-based explicit compressible solver
- **`explicitDBNSSRFSolver`**: density-based explicit compressible solver in single rotating reference frame using the absolute velocity formulation
- **`cpbnsLTS`**: steady-state pressure-based compressible solver
- **`cpbnsPIMPLE`**: transient pressure-based compressible solver
### heat-transfer:
- **`buoyantBoussinesqSimpleSolver`**: SIMPLE-based incompressible steady-state solver with buoyant body-force based on Boussinesq approximation
- **`buoyantSimpleSolver`**: SIMPLE-based incompressible steady-state solver with buoyant body-force
### multiphase:
- **`vofSolver`**: Volume of Fluid (VOF) phase-fraction based solver for two incompressible, isothermal immiscible fluids
- **`vofDyMSolver`**: Volume of Fluid (VOF) phase-fraction based solver for two incompressible, isothermal immiscible fluids with dynamic mesh solver
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
#### Wall Damping:
- van Driest

## Installation
Installers available for download at <http://www.caelus-cml.com/download/>.

## Execution Environment
Caelus provides an execution environment based on Python and does not require the sourcing or calling an environment file for every new shell. This environment operates in nearly identical manner to the previous except every command needs is prepended with caelus.py. For parallel, it simplifies usage of utilities and solver. For example, using pCorrSimpleSolver:

**Previous**
```Bash
 $> mpiexec -n 4 pCorrSimpleSolver -parallel &> pCorrSimpleSolver.log &
```
**New**:
```Bash
 $> caelus.py -l pCorrSimpleSolver -parallel
```
Type `caelus.py --help` for the complete list of options.

### Windows
- The Windows installer is not digitally signed and may cause some browsers to flag the installer as containing a virus. If this occurs, accept the download or try a different browser.
- Install the prerequisites first, starting with Python. The prerequisites package is an archive that contains four installation packages.
- During the Python installation choose to add Python to the PATH system environment variable. Otherwise the Python bin directory needs to be added to the PATH variable manually.
- For Windows 8, the MPICH2 software requires .NET 2.050727 to be installed. This is provided in the prerequisites package.
- For MPICH2 installation see below.
- To install Caelus, double click the .exe and follow the on screen prompts.
    * If you use the new method to run Caelus you do not need to do anything further to set up Caelus other than double clicking the icon on the desktop.
    * If you want use the previous method to run Caelus then you need to execute ‘call {Caelus install path}/etc/caelus-environment.cmd’ from a command prompt to configure the run environment.
    
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
    * New method (RECOMMENDED): The installer adds paths to the PATH and PYTHONPATH environment and sets them in the users .bashrc file
    * Previous method: Source the environment file created by the installer. For users of the Bash shell, the caelus-bashrc from the etc directory is provided (source etc/caelus-bashrc). For users of the C shell, the caelus-cshrc from the etc directory is provided (source etc/caelus-cshrc). For users of other shells, use the existing shell files as a template to create an appropriate environment file.
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
    * New method (RECOMMENDED): The installer adds paths to the PATH and PYTHONPATH environment and sets them in the users .bashrc file
    * Previous method: Source the environment file created by the installer. For users of the Bash shell, the caelus-bashrc from the etc directory is provided (source etc/caelus-bashrc). For users of the C shell, the caelus-cshrc from the etc directory is provided (source etc/caelus-cshrc). For users of other shells, use the existing shell files as a template to create an appropriate environment file.
    
### Contributors Repository Set Up
There is a public git repository for the Caelus source code that tracks with the current release.

<https://bitbucket.org/appliedccm/caelus-contributors>

To make a local copy, clone the repository with git:

```Bash
 $> git clone git@bitbucket.org:appliedccm/caelus-contributors.git
```
Some environment variables need to be set manually. These are set automatically by the installer with the packaged versions.
Add to your PATH ./external/scons-local-2.3.4 and ./bin
Set the variable PYTHONPATH to ./lib/python2.6

To set up the development environment, copy the conf_template.py file in ./lib/python2.6/Caelus to conf.py in the same directory. Modify the conf.py to match the paths on your system. In particular the PROJECT_NAME, PROJECT_VER, PROJECT_DIR and the MPI vars. Once this is done, run:

```Bash
 $> python -s lib/python2.6/Caelus/write_env.py
```
to rebuild the environment scripts. This only needs to be done once. This manual step is required because the scripts are automatically generated by the installer with the packaged versions. Note: Caelus currently requires a python 2.x. You will also need to set the PYTHONPATH variable to ./lib/python2.6 before executing the write_env.py script.

Then source the appropriate script: 

#### Linux/Mac
```Bash
 $> . etc/caelus-bashrc
```
#### Windows
```Bash
 $> call etc\caelus-environment.cmd
```

Finally, use SCons to build Caelus:

```Bash
 $> scons.py install
```

Provide the -h to get a full list of build options or refer to the scons_notes.md for further details about the build system.
