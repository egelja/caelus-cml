/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

InClass
    CML::basicRhoThermo

Description
    Macros for creating 'basic' density-based thermo packages

\*---------------------------------------------------------------------------*/

#ifndef makeBasicRhoThermo_H
#define makeBasicRhoThermo_H

#include "basicRhoThermo.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBasicRhoThermo(Cthermo,Mixture,Transport,Thermo,EqnOfState)       \
                                                                              \
typedef Cthermo<Mixture<Transport<specieThermo<Thermo<EqnOfState> > > > >     \
    Cthermo##Mixture##Transport##Thermo##EqnOfState;                          \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    Cthermo##Mixture##Transport##Thermo##EqnOfState,                          \
    #Cthermo                                                                  \
        "<"#Mixture"<"#Transport"<specieThermo<"#Thermo"<"#EqnOfState">>>>>", \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicRhoThermo,                                                           \
    Cthermo##Mixture##Transport##Thermo##EqnOfState,                          \
    fvMesh                                                                    \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicThermo,                                                              \
    Cthermo##Mixture##Transport##Thermo##EqnOfState,                          \
    fvMesh                                                                    \
);


#define makeBasicRhoPolyThermo(Cthermo,Mixture,Order)                         \
                                                                              \
typedef polynomialTransport                                                   \
<                                                                             \
    specieThermo                                                              \
    <                                                                         \
        hPolynomialThermo                                                     \
        <                                                                     \
            icoPolynomial<Order>,                                             \
            Order                                                             \
            >                                                                 \
    >,                                                                        \
    Order                                                                     \
> icoPoly##Order##ThermoPhysics;                                              \
                                                                              \
typedef Cthermo<Mixture<icoPoly##Order##ThermoPhysics> >                      \
    Cthermo##Mixture##icoPoly##Order##ThermoPhysics;                          \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    Cthermo##Mixture##icoPoly##Order##ThermoPhysics,                          \
    #Cthermo"<"#Mixture"<icoPoly"#Order"ThermoPhysics>>",                     \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicRhoThermo,                                                           \
    Cthermo##Mixture##icoPoly##Order##ThermoPhysics,                          \
    fvMesh                                                                    \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicThermo,                                                              \
    Cthermo##Mixture##icoPoly##Order##ThermoPhysics,                          \
    fvMesh                                                                    \
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
