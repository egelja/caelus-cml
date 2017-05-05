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

#include "linearDamper.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace restraints
{
    defineTypeNameAndDebug(linearDamper, 0);

    addToRunTimeSelectionTable
    (
        restraint,
        linearDamper,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::restraints::linearDamper::linearDamper
(
    const word& name,
    const dictionary& dict,
    const rigidBodyModel& model
)
:
    restraint(name, dict, model)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::restraints::linearDamper::~linearDamper()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::restraints::linearDamper::restrain
(
    scalarField& tau,
    Field<spatialVector>& fx
) const
{
    vector force = -coeff_*model_.v(model_.master(bodyID_)).l();

    if (model_.debug)
    {
        Info<< " force " << force << endl;
    }

    // Accumulate the force for the restrained body
    fx[bodyIndex_] += spatialVector(Zero, force);
}


bool CML::RBD::restraints::linearDamper::read
(
    const dictionary& dict
)
{
    restraint::read(dict);

    coeffs_.lookup("coeff") >> coeff_;

    return true;
}


void CML::RBD::restraints::linearDamper::write
(
    Ostream& os
) const
{
    restraint::write(os);

    os.writeKeyword("coeff")
        << coeff_ << token::END_STATEMENT << nl;
}


//Write state
void CML::RBD::restraints::linearDamper::writeState
(
    autoPtr<CML::OFstream> &stateFilePtr
) const
{
}

// ************************************************************************* //
