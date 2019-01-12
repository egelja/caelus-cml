/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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

#include "joint.hpp"
#include "rigidBodyModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
    defineTypeNameAndDebug(joint, 0);
    defineRunTimeSelectionTable(joint, dictionary);
}
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::RBD::joint> CML::RBD::joint::New(joint* jointPtr)
{
    return autoPtr<joint>(jointPtr);
}


CML::autoPtr<CML::RBD::joint> CML::RBD::joint::New
(
    const dictionary& dict
)
{
    const word bodyType(dict.lookup("type"));

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(bodyType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown joint type "
            << bodyType << nl << nl
            << "Valid joint types are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<joint>(cstrIter()(dict));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::joint::~joint()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::joint::write(Ostream& os) const
{
    os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
}


// ************************************************************************* //
