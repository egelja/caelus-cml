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

#include "magGrad.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace calcTypes
    {
        defineTypeNameAndDebug(magGrad, 0);
        addToRunTimeSelectionTable(calcType, magGrad, dictionary);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::calcTypes::magGrad::magGrad()
:
    calcType()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::calcTypes::magGrad::~magGrad()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::calcTypes::magGrad::init()
{
    argList::validArgs.append("magGrad");
    argList::validArgs.append("fieldName");
}


void CML::calcTypes::magGrad::preCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{}


void CML::calcTypes::magGrad::calc
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

        writeMagGradField<scalar>(fieldHeader, mesh, processed);
        writeMagGradField<vector>(fieldHeader, mesh, processed);

        if (!processed)
        {
            FatalError
                << "Unable to process " << fieldName << nl
                << "No call to magGrad for fields of type "
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

