/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    Decomposition given a cell-to-processor association in a file

\*---------------------------------------------------------------------------*/

#include "manualDecomp.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "IFstream.hpp"
#include "labelIOList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(manualDecomp, 0);

    addToRunTimeSelectionTable
    (
        decompositionMethod,
        manualDecomp,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::manualDecomp::manualDecomp(const dictionary& decompositionDict)
:
    decompositionMethod(decompositionDict),
    decompDataFile_
    (
        decompositionDict.subDict(word(decompositionDict.lookup("method"))
      + "Coeffs").lookup("dataFile")
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::labelList CML::manualDecomp::decompose
(
    const polyMesh& mesh,
    const pointField& points,
    const scalarField& pointWeights
)
{
    labelIOList finalDecomp
    (
        IOobject
        (
            decompDataFile_,
            mesh.facesInstance(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE,
            false
        )
    );

    // check if the final decomposition is OK

    if (finalDecomp.size() != points.size())
    {
        FatalErrorInFunction
            << "Size of decomposition list does not correspond "
            << "to the number of points.  Size: "
            << finalDecomp.size() << " Number of points: "
            << points.size()
            << ".\n" << "Manual decomposition data read from file "
            << decompDataFile_ << "." << endl
            << exit(FatalError);
    }

    if (min(finalDecomp) < 0 || max(finalDecomp) > nProcessors_ - 1)
    {
        FatalErrorInFunction
            << "According to the decomposition, cells assigned to "
            << "impossible processor numbers.  Min processor = "
            << min(finalDecomp) << " Max processor = " << max(finalDecomp)
            << ".\n" << "Manual decomposition data read from file "
            << decompDataFile_ << "." << endl
            << exit(FatalError);
    }

    return finalDecomp;
}


// ************************************************************************* //
