/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

Typedefs
    CML::thermoPhysicsTypes

Description
    Type definitions for thermo-physics models

\*---------------------------------------------------------------------------*/

#ifndef thermoPhysicsTypes_H
#define thermoPhysicsTypes_H

#include "perfectGas.hpp"
#include "isobaricPerfectGas.hpp"
#include "hConstThermo.hpp"
#include "janafThermo.hpp"
#include "specieThermo.hpp"
#include "sutherlandTransport.hpp"
#include "constTransport.hpp"

#include "icoPolynomial.hpp"
#include "hPolynomialThermo.hpp"
#include "polynomialTransport.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef constTransport<specieThermo<hConstThermo<perfectGas> > >
        constGasThermoPhysics;

    typedef sutherlandTransport<specieThermo<janafThermo<perfectGas> > >
        gasThermoPhysics;

    typedef constTransport<specieThermo<hConstThermo<isobaricPerfectGas> > >
        constIsobaricGasThermoPhysics;

    typedef sutherlandTransport<specieThermo<janafThermo<isobaricPerfectGas> > >
        isobaricGasThermoPhysics;

    typedef polynomialTransport
        <
            specieThermo
            <
                hPolynomialThermo
                <
                    icoPolynomial<8>,
                    8
                >
            >,
            8
        >
        icoPoly8ThermoPhysics;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
