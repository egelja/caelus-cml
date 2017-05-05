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

#include "magSqr.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace calcTypes
    {
        defineTypeNameAndDebug(magSqr, 0);
        addToRunTimeSelectionTable(calcType, magSqr, dictionary);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::calcTypes::magSqr::magSqr()
:
    calcType()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::calcTypes::magSqr::~magSqr()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::calcTypes::magSqr::init()
{
    argList::validArgs.append("magSqr");
    argList::validArgs.append("fieldName");
}


void CML::calcTypes::magSqr::preCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{}


void CML::calcTypes::magSqr::calc
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

        writeMagSqrField<scalar>(fieldHeader, mesh, processed);
        writeMagSqrField<vector>(fieldHeader, mesh, processed);
        writeMagSqrField<sphericalTensor>(fieldHeader, mesh, processed);
        writeMagSqrField<symmTensor>(fieldHeader, mesh, processed);
        writeMagSqrField<tensor>(fieldHeader, mesh, processed);

        if (!processed)
        {
            FatalError
                << "Unable to process " << fieldName << nl
                << "No call to magSqr for fields of type "
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

