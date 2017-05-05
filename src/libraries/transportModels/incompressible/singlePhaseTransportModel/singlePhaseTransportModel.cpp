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

#include "singlePhaseTransportModel.hpp"
#include "viscosityModel.hpp"
#include "volFields.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::singlePhaseTransportModel::singlePhaseTransportModel
(
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    transportModel(U, phi),
    viscosityModelPtr_(viscosityModel::New("nu", *this, U, phi))
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

CML::singlePhaseTransportModel::~singlePhaseTransportModel()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField> CML::singlePhaseTransportModel::nu() const
{
    return viscosityModelPtr_->nu();
}


void CML::singlePhaseTransportModel::correct()
{
    viscosityModelPtr_->correct();
}


bool CML::singlePhaseTransportModel::read()
{
    if (regIOobject::read())
    {
        return viscosityModelPtr_->read(*this);
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
