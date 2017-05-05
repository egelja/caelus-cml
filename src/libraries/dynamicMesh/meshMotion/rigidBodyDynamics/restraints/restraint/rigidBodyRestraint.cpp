/*---------------------------------------------------------------------------* \
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

#include "rigidBodyRestraint.hpp"
#include "rigidBodyModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
    defineTypeNameAndDebug(restraint, 0);
    defineRunTimeSelectionTable(restraint, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::restraint::restraint
(
    const word& name,
    const dictionary& dict,
    const rigidBodyModel& model
)
:
    name_(name),
    bodyID_(model.bodyID(dict.lookup("body"))),
    bodyIndex_(model.master(bodyID_)),
    coeffs_(dict),
    model_(model)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::restraint::~restraint()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const CML::dictionary& CML::RBD::restraint::coeffDict() const
{
    return coeffs_;
}


bool CML::RBD::restraint::read(const dictionary& dict)
{
    coeffs_ = dict;
    return true;
}


void CML::RBD::restraint::write(Ostream& os) const
{
    os.writeKeyword("type")
        << type() << token::END_STATEMENT << nl;
    os.writeKeyword("body")
        << model_.name(bodyID_) << token::END_STATEMENT << nl;
}

void CML::RBD::restraint::writeState(autoPtr<CML::OFstream> &stateFilePtr) const
{
}


// ************************************************************************* //
