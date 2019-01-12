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

#include "laminarFlameSpeed.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::laminarFlameSpeed> CML::laminarFlameSpeed::New
(
    const psiuReactionThermo& ct
)
{
    // Do not register the dictionary
    IOdictionary propDict
    (
        IOobject
        (
            "combustionProperties",
            ct.T().time().constant(),
            ct.T().db(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE,
            false
        )
    );

    const word corrType(propDict.lookup("laminarFlameSpeedCorrelation"));

    Info<< "Selecting laminar flame speed correlation " << corrType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(corrType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction
        (
            propDict
        )   << "Unknown laminarFlameSpeed type "
            << corrType << nl << nl
            << "Valid laminarFlameSpeed types are :" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<laminarFlameSpeed>(cstrIter()(propDict, ct));
}
