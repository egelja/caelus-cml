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
    CML::basicMixture

Description

\*---------------------------------------------------------------------------*/

#ifndef makeBasicMixture_H
#define makeBasicMixture_H

#include "basicMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBasicMixture(Mixture,Transport,Thermo,EqnOfState)                 \
                                                                              \
typedef Mixture<Transport<specieThermo<Thermo<EqnOfState> > > >               \
    Mixture##Transport##Thermo##EqnOfState;                                   \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
    (Mixture##Transport##Thermo##EqnOfState,                                  \
    #Mixture"<"#Transport"<specieThermo<"#Thermo"<"#EqnOfState">>>>", 0)


#define makeBasicPolyMixture(Mixture,Order)                                   \
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
typedef Mixture<icoPoly##Order##ThermoPhysics>                                \
    Mixture##icoPoly##Order##ThermoPhysics;                                   \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
    (Mixture##icoPoly##Order##ThermoPhysics,                                  \
    #Mixture"<icoPoly"#Order"ThermoPhysics>", 0)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
