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

\*---------------------------------------------------------------------------*/

#include "makeSolidMixtureThermo.hpp"

#include "constRho.hpp"

#include "constSolidThermo.hpp"
#include "exponentialSolidThermo.hpp"

#include "constSolidTransport.hpp"
#include "exponentialSolidTransport.hpp"

#include "constSolidRad.hpp"

#include "basicSolidThermo.hpp"

#include "multiComponentSolidMixture.hpp"
#include "reactingSolidMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

makeSolidMixtureThermo
(
    basicSolidThermo,
    solidMixtureThermo,
    multiComponentSolidMixture,
    constSolidTransport,
    constSolidRad,
    constSolidThermo,
    constRho
);

makeSolidMixtureThermo
(
    basicSolidThermo,
    solidMixtureThermo,
    multiComponentSolidMixture,
    exponentialSolidTransport,
    constSolidRad,
    exponentialSolidThermo,
    constRho
);

makeSolidMixtureThermo
(
    basicSolidThermo,
    solidMixtureThermo,
    reactingSolidMixture,
    exponentialSolidTransport,
    constSolidRad,
    exponentialSolidThermo,
    constRho
);

makeSolidMixtureThermo
(
    basicSolidThermo,
    solidMixtureThermo,
    reactingSolidMixture,
    constSolidTransport,
    constSolidRad,
    constSolidThermo,
    constRho
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
