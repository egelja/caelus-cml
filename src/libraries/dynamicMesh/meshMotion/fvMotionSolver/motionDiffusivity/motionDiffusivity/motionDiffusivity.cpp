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

#include "motionDiffusivity.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(motionDiffusivity, 0);
    defineRunTimeSelectionTable(motionDiffusivity, Istream);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::motionDiffusivity::motionDiffusivity(const fvMesh& mesh)
:
    mesh_(mesh)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::motionDiffusivity> CML::motionDiffusivity::New
(
    const fvMesh& mesh,
    Istream& mdData
)
{
    const word motionType(mdData);

    Info<< "Selecting motion diffusion: " << motionType << endl;

    IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(motionType);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown diffusion type "
            << motionType << nl << nl
            << "Valid diffusion types are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<motionDiffusivity>(cstrIter()(mesh, mdData));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::motionDiffusivity::~motionDiffusivity()
{}


// ************************************************************************* //
