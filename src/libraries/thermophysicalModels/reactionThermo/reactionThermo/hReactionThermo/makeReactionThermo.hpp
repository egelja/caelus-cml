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
    CML::hReactionThermo

\*---------------------------------------------------------------------------*/

#ifndef makeReactionThermo_H
#define makeReactionThermo_H

#include "addToRunTimeSelectionTable.hpp"
#include "basicRhoThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeReactionThermo\
(                                                                             \
    CThermo,                                                                  \
    MixtureThermo,                                                            \
    Mixture,                                                                  \
    Transport,                                                                \
    Thermo,                                                                   \
    EqnOfState                                                                \
)                                                                             \
                                                                              \
typedef MixtureThermo                                                         \
    <Mixture<Transport<specieThermo<Thermo<EqnOfState> > > > >                \
    MixtureThermo##Mixture##Transport##Thermo##EqnOfState;                    \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    MixtureThermo##Mixture##Transport##Thermo##EqnOfState,                    \
    #MixtureThermo                                                            \
        "<"#Mixture"<"#Transport"<specieThermo<"#Thermo"<"#EqnOfState">>>>>", \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicRhoThermo,                                                           \
    MixtureThermo##Mixture##Transport##Thermo##EqnOfState,                    \
    fvMesh                                                                    \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    CThermo,                                                                  \
    MixtureThermo##Mixture##Transport##Thermo##EqnOfState,                    \
    fvMesh                                                                    \
)


#define makeReactionMixtureThermo(CThermo,MixtureThermo,Mixture,ThermoPhys)   \
                                                                              \
typedef MixtureThermo<Mixture<ThermoPhys> >                                   \
    MixtureThermo##Mixture##ThermoPhys;                                       \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    MixtureThermo##Mixture##ThermoPhys,                                       \
    #MixtureThermo"<"#Mixture"<"#ThermoPhys">>",                              \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicRhoThermo,                                                           \
    MixtureThermo##Mixture##ThermoPhys,                                       \
    fvMesh                                                                    \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    CThermo,                                                                  \
    MixtureThermo##Mixture##ThermoPhys,                                       \
    fvMesh                                                                    \
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
