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

#include "interpolate.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace calcTypes
    {
        defineTypeNameAndDebug(interpolate, 0);
        addToRunTimeSelectionTable(calcType, interpolate, dictionary);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::calcTypes::interpolate::interpolate()
:
    calcType()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::calcTypes::interpolate::~interpolate()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::calcTypes::interpolate::init()
{
    CML::argList::validArgs.append("interpolate");
    argList::validArgs.append("fieldName");
}


void CML::calcTypes::interpolate::preCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{}


void CML::calcTypes::interpolate::calc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{
    const word fieldName = args[2];

    IOobject fieldHeader
    (
        fieldName,
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
    );

    // Check field exists
    if (fieldHeader.headerOk())
    {
        bool processed = false;

        writeInterpolateField<scalar>(fieldHeader, mesh, processed);
        writeInterpolateField<vector>(fieldHeader, mesh, processed);
        writeInterpolateField<sphericalTensor>(fieldHeader, mesh, processed);
        writeInterpolateField<symmTensor>(fieldHeader, mesh, processed);
        writeInterpolateField<tensor>(fieldHeader, mesh, processed);

        if (!processed)
        {
            FatalError
                << "Unable to process " << fieldName << nl
                << "No call to interpolate for fields of type "
                << fieldHeader.headerClassName() << nl << nl
                << exit(FatalError);
        }
    }
    else
    {
        Info<< "    No " << fieldName << endl;
    }
}


// ************************************************************************* //

