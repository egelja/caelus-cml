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

#include "rigidBody.hpp"
#include "subBody.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
    defineTypeNameAndDebug(rigidBody, 0);
    defineRunTimeSelectionTable(rigidBody, dictionary);

    addToRunTimeSelectionTable
    (
        rigidBody,
        rigidBody,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::autoPtr<CML::RBD::rigidBody> CML::RBD::rigidBody::clone() const
{
    return autoPtr<rigidBody>(new rigidBody(*this));
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::RBD::rigidBody> CML::RBD::rigidBody::New
(
    const word& name,
    const scalar& m,
    const vector& c,
    const symmTensor& Ic
)
{
    return autoPtr<rigidBody>(new rigidBody(name, m, c, Ic));
}


CML::autoPtr<CML::RBD::rigidBody> CML::RBD::rigidBody::New
(
    const word& name,
    const dictionary& dict
)
{
    const word bodyType(dict.lookup("type"));

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(bodyType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown rigidBody type "
            << bodyType << nl << nl
            << "Valid rigidBody types are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<rigidBody>(cstrIter()(name, dict));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::rigidBody::~rigidBody()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::RBD::rigidBody::massless() const
{
    return false;
}


void CML::RBD::rigidBody::merge(const subBody& subBody)
{
    *this = rigidBody
    (
        name(),
        *this + transform(subBody.masterXT(), subBody.body())
    );
}


void CML::RBD::rigidBody::write(Ostream& os) const
{
    os.writeKeyword("type")
        << type() << token::END_STATEMENT << nl;

    os.writeKeyword("mass")
        << m() << token::END_STATEMENT << nl;

    os.writeKeyword("centreOfMass")
        << c() << token::END_STATEMENT << nl;

    os.writeKeyword("inertia")
        << Ic() << token::END_STATEMENT << nl;
}


// ************************************************************************* //
