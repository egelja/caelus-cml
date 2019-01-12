/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License

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

#include "reactionRateFlameArea.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(reactionRateFlameArea, 0);
    defineRunTimeSelectionTable(reactionRateFlameArea, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::reactionRateFlameArea::reactionRateFlameArea
(
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh,
    const combustionModel& combModel
)
:
    coeffDict_(dict.subDict(modelType + "Coeffs")),
    mesh_(mesh),
    combModel_(combModel),
    fuel_(dict.lookup("fuel")),
    omega_
    (
        IOobject
        (
            "FSDomega",
            mesh_.time().timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    )
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::reactionRateFlameArea::read(const dictionary& dict)
{
    dict.lookup("fuel") >> fuel_;

    return true;
}
