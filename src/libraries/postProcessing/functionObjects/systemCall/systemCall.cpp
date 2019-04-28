/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
 License
 
    This file is part of Caelus.
 
    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "systemCall.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(systemCall, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::systemCall::systemCall
(
    const word& name,
    const objectRegistry&,
    const dictionary& dict,
    const bool
)
:
    name_(name),
    executeCalls_(),
    endCalls_(),
    writeCalls_()
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::systemCall::~systemCall()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::systemCall::read(const dictionary& dict)
{
    dict.readIfPresent("executeCalls", executeCalls_);
    dict.readIfPresent("endCalls", endCalls_);
    dict.readIfPresent("writeCalls", writeCalls_);

    if (executeCalls_.empty() && endCalls_.empty() && writeCalls_.empty())
    {
        WarningInFunction
            << "no executeCalls, endCalls or writeCalls defined."
            << endl;
    }
}


void CML::systemCall::execute()
{
    forAll(executeCalls_, callI)
    {
        CML::system(executeCalls_[callI]);
    }
}


void CML::systemCall::end()
{
    forAll(endCalls_, callI)
    {
        CML::system(endCalls_[callI]);
    }
}


void CML::systemCall::timeSet()
{
    // Do nothing
}


void CML::systemCall::write()
{
    forAll(writeCalls_, callI)
    {
        CML::system(writeCalls_[callI]);
    }
}


// ************************************************************************* //
