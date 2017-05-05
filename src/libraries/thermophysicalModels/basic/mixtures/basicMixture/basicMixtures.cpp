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

Description
    Mixture instantiation

\*---------------------------------------------------------------------------*/

#include "error.hpp"

#include "basicMixture.hpp"
#include "makeBasicMixture.hpp"

#include "perfectGas.hpp"
#include "incompressible.hpp"
#include "isobaricPerfectGas.hpp"

#include "eConstThermo.hpp"

#include "hConstThermo.hpp"
#include "janafThermo.hpp"
#include "specieThermo.hpp"

#include "constTransport.hpp"
#include "sutherlandTransport.hpp"

#include "icoPolynomial.hpp"
#include "hPolynomialThermo.hpp"
#include "polynomialTransport.hpp"

#include "pureMixture.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

makeBasicMixture
(
    pureMixture,
    constTransport,
    eConstThermo,
    perfectGas
);

makeBasicMixture
(
    pureMixture,
    sutherlandTransport,
    eConstThermo,
    perfectGas
);


makeBasicMixture
(
    pureMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeBasicMixture
(
    pureMixture,
    sutherlandTransport,
    hConstThermo,
    perfectGas
);

makeBasicMixture
(
    pureMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeBasicMixture
(
    pureMixture,
    constTransport,
    hConstThermo,
    incompressible
);

makeBasicPolyMixture
(
    pureMixture,
    3
);

makeBasicPolyMixture
(
    pureMixture,
    8
);


makeBasicMixture
(
    pureMixture,
    constTransport,
    hConstThermo,
    isobaricPerfectGas
);

makeBasicMixture
(
    pureMixture,
    sutherlandTransport,
    hConstThermo,
    isobaricPerfectGas
);

makeBasicMixture
(
    pureMixture,
    sutherlandTransport,
    janafThermo,
    isobaricPerfectGas
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
