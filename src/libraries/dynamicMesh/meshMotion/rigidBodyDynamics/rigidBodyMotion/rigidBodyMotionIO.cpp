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

#include "rigidBodyMotion.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::RBD::rigidBodyMotion::read(const dictionary& dict)
{
    rigidBodyModel::read(dict);

    aRelax_ = dict.lookupOrDefault<scalar>("accelerationRelaxation", 1.0);
    aDamp_ = dict.lookupOrDefault<scalar>("accelerationDamping", 1.0);
    report_ = dict.lookupOrDefault<Switch>("report", false);

    return true;
}


void CML::RBD::rigidBodyMotion::write(Ostream& os) const
{
    rigidBodyModel::write(os);

    os.writeKeyword("accelerationRelaxation")
        << aRelax_ << token::END_STATEMENT << nl;
    os.writeKeyword("accelerationDamping")
        << aDamp_ << token::END_STATEMENT << nl;
    os.writeKeyword("report")
        << report_ << token::END_STATEMENT << nl;
}


// ************************************************************************* //
