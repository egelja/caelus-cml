/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "constant.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace laminarFlameSpeedModels
{
    defineTypeNameAndDebug(constant, 0);

    addToRunTimeSelectionTable
    (
        laminarFlameSpeed,
        constant,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::laminarFlameSpeedModels::constant::constant
(
    const dictionary& dict,
    const psiuReactionThermo& ct
)
:
    laminarFlameSpeed(dict, ct),

    Su_(dict.lookup("Su"))
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::laminarFlameSpeedModels::constant::operator()() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "Su0",
                psiuReactionThermo_.T().time().timeName(),
                psiuReactionThermo_.T().db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            psiuReactionThermo_.T().mesh(),
            Su_
        )
    );
}
