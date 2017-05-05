/*---------------------------------------------------------------------------*\
Copyright (C) 2017 Applied CCM Pty Ltd
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

#include "externalForceMoment.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace restraints
{
    defineTypeNameAndDebug(externalForceMoment, 0);

    addToRunTimeSelectionTable
    (
        restraint,
        externalForceMoment,
        dictionary
    );
}
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::restraints::externalForceMoment::externalForceMoment
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

CML::RBD::restraints::externalForceMoment::~externalForceMoment()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::restraints::externalForceMoment::restrain
(
    scalarField& tau,
    Field<spatialVector>& fx
) const
{
    // Application point in global frame
    point applicationPt = bodyPoint(refApplicationPt_);

    // Force and moment on the master body
    vector force(Zero);
    vector moment(Zero);

    if (global_)
    {
       // User supplied force and moment is already in global frame
       force = force_;
       moment = moment_;       
    }
    else
    {
       // User supplied force and moment is in body frame and needs to be transformed
       spatialVector fGlobal = bodyForce(moment_, force_);
       force = fGlobal.l();
       moment = fGlobal.w();
    }

    // Add additional moment from force
    moment += applicationPt ^ force;

    if (model_.debug)
    {
        Info<< " applicationPt " << applicationPt
            << " force " << force 
            << " moment " << moment
            << endl;
    }

    // Accumulate the force for the restrained body
    fx[bodyIndex_] += spatialVector(moment, force);
}


bool CML::RBD::restraints::externalForceMoment::read
(
    const dictionary& dict
)
{
    restraint::read(dict);

    coeffs_.lookup("refApplicationPt") >> refApplicationPt_;

    global_ = coeffs_.lookupOrDefault("global", false);

    coeffs_.lookup("force") >> force_;

    coeffs_.lookup("moment") >> moment_;

    return true;
}


void CML::RBD::restraints::externalForceMoment::write
(
    Ostream& os
) const
{
    restraint::write(os);

    os.writeKeyword("refApplicationPt")
        << refApplicationPt_ << token::END_STATEMENT << nl;

    os.writeKeyword("global")
        << global_ << token::END_STATEMENT << nl;

    os.writeKeyword("force")
        << force_ << token::END_STATEMENT << nl;

    os.writeKeyword("moment")
        << moment_ << token::END_STATEMENT << nl;
}


//Write state
void CML::RBD::restraints::externalForceMoment::writeState
(
    autoPtr<CML::OFstream> &stateFilePtr
) const
{
}


// ************************************************************************* //
