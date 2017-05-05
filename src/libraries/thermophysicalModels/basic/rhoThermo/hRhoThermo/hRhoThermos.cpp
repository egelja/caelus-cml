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

\*---------------------------------------------------------------------------*/

#include "makeBasicRhoThermo.hpp"

#include "perfectGas.hpp"
#include "isobaricPerfectGas.hpp"
#include "incompressible.hpp"

#include "hConstThermo.hpp"
#include "janafThermo.hpp"
#include "specieThermo.hpp"

#include "constTransport.hpp"
#include "sutherlandTransport.hpp"

#include "icoPolynomial.hpp"
#include "hPolynomialThermo.hpp"
#include "polynomialTransport.hpp"

#include "hRhoThermo.hpp"
#include "pureMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

makeBasicRhoThermo
(
    hRhoThermo,
    pureMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeBasicRhoThermo
(
    hRhoThermo,
    pureMixture,
    sutherlandTransport,
    hConstThermo,
    perfectGas
);

makeBasicRhoThermo
(
    hRhoThermo,
    pureMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeBasicRhoThermo
(
    hRhoThermo,
    pureMixture,
    constTransport,
    hConstThermo,
    incompressible
);

makeBasicRhoPolyThermo
(
    hRhoThermo,
    pureMixture,
    3
);

makeBasicRhoPolyThermo
(
    hRhoThermo,
    pureMixture,
    8
);

makeBasicRhoThermo
(
    hRhoThermo,
    pureMixture,
    constTransport,
    hConstThermo,
    isobaricPerfectGas
);

makeBasicRhoThermo
(
    hRhoThermo,
    pureMixture,
    sutherlandTransport,
    hConstThermo,
    isobaricPerfectGas
);

makeBasicRhoThermo
(
    hRhoThermo,
    pureMixture,
    sutherlandTransport,
    janafThermo,
    isobaricPerfectGas
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
