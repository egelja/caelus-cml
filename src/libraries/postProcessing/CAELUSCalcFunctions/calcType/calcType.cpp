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

#include "calcType.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(calcType, 0);

defineRunTimeSelectionTable(calcType, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::calcType::calcType()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::calcType::~calcType()
{}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::calcType::init()
{
    // Do nothing
}


void CML::calcType::preCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{
    // Do nothing
}


void CML::calcType::calc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{
    // Do nothing
}


void CML::calcType::postCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{
    // Do nothing
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::calcType::tryInit()
{
    FatalIOError.throwExceptions();

    try
    {
        init();
    }
    catch(IOerror& err)
    {
        Warning<< err << endl;
    }
}


void CML::calcType::tryPreCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{
    FatalIOError.throwExceptions();

    try
    {
        preCalc(args, runTime, mesh);
    }
    catch(IOerror& err)
    {
        Warning<< err << endl;
    }
}


void CML::calcType::tryCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{
    FatalIOError.throwExceptions();

    try
    {
        calc(args, runTime, mesh);
    }
    catch(IOerror& err)
    {
        Warning<< err << endl;
    }
}


void CML::calcType::tryPostCalc
(
    const argList& args,
    const Time& runTime,
    const fvMesh& mesh
)
{
    FatalIOError.throwExceptions();

    try
    {
        postCalc(args, runTime, mesh);
    }
    catch(IOerror& err)
    {
        Warning<< err << endl;
    }
}


// ************************************************************************* //
