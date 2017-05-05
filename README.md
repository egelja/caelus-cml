# Caelus

## License

Caelus has been forked from OpenFOAM-2.1.1. It is currently compatible with OpenFOAM.

It is released under the GPL: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

## Contact
- Aleks Jemcov: a.jemcov@appliedccm.com
- Chris Sideroff: c.sideroff@appliedccm.ca
- Darrin Stephens: d.stephens@appliedccm.com.au

## COPYRIGHT Applied CCM 2014

Current version: Caelus 4.10

## Solvers
### basic:
- potenialSolver: potential flow solver provided by the OpenFOAM Foundation
### incompressible:
- simpleSolver: SIMPLE-based incompressible steady-state solver provided by the OpenFOAM Foundation
- SRFSimpleSolver: SIMPLE-based incompressible steady-state solver in a single rotating rotating reference frame provided by the OpenFOAM Foundation
- pimpleSolver: PIMPLE-based incompressible transient solver provided by the OpenFOAM Foundation
- SRFPimpleSolver: PIMPLE-based incompressible transient solver in a single rotating reference frame provided by the OpenFOAM Foundation
- pisoSolver: PISO-based incompressible transient solver provided by the OpenFOAM Foundation
### heat-transfer:
- buoyantBoussinesqSimpleSolver - SIMPLE-based incompressible steady-state solver with buoyant body-force based on Boussinesq approximation provided by the OpenFOAM Foundation
- buoyantSimpleSolver - SIMPLE-based incompressible steady-state solver with buoyant body-force provided by the OpenFOAM Foundation
## Turbulence Models
### Reynolds Avergaged Navier-Stokes:
- Spalart-Allmaras with curvature correction
- Realizable k-epsilon
- k-omega SST (Menter et al 2003)
#### Wall Functions:
- nutU high Reynolds number velocity based
- nutUSpalding all Reynolds number velocity based
### Detached Eddy Simulation:
- Spalart-Allmaras Delayed DES
- Spalart-Allmaras Improved Delayed DES
### Large Eddy Simulation:
- Smagorinsky
- dynamic sub-grid scale kinetic energy
#### Wall Damping:
- van Driest
