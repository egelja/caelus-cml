/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2015 OpenFOAM Foundation
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

#include "MRFSource.hpp"
#include "fvMesh.hpp"
#include "fvMatrices.hpp"
#include "MRFZone.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace fv
{
    defineTypeNameAndDebug(MRFSource, 0);
    addToRunTimeSelectionTable
    (
        option,
        MRFSource,
        dictionary
    );
}
}

void CML::fv::MRFSource::initialise()
{
    if (selectionMode_ != smCellZone)
    {
        FatalErrorInFunction
            << "The MRF region must be specified as a cellZone.  Current "
            << "selection mode is " << selectionModeTypeNames_[selectionMode_]
            << exit(FatalError);
    }

    mrfPtr_.reset
    (
        new MRFZone
        (
            name_,
            mesh_,
            coeffs_,
            cellSetName_
        )
    );

    const volVectorField& U = mesh_.lookupObject<volVectorField>(UName_);

    mrfPtr_->correctBoundaryVelocity(const_cast<volVectorField&>(U));

    fieldNames_.setSize(1, UName_);
    applied_.setSize(1, false);
}


CML::fv::MRFSource::MRFSource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(name, modelType, dict, mesh),
    mrfPtr_(nullptr),
    UName_(coeffs_.lookupOrDefault<word>("UName", "U"))
{
    initialise();
}


void CML::fv::MRFSource::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    // Update the velocity boundary conditions for changes in rotation speed
    mrfPtr_->correctBoundaryVelocity(const_cast<volVectorField&>(eqn.psi()));

    // Add to rhs of equation
    mrfPtr_->addCoriolis(eqn, true);
}


void CML::fv::MRFSource::addSup
(
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    // Update the velocity boundary conditions for changes in rotation speed
    mrfPtr_->correctBoundaryVelocity(const_cast<volVectorField&>(eqn.psi()));

    // Add to rhs of equation
    mrfPtr_->addCoriolis(rho, eqn, true);
}


void CML::fv::MRFSource::makeRelative(surfaceScalarField& phi) const
{
    mrfPtr_->makeRelative(phi);
}


void CML::fv::MRFSource::makeRelative
(
    FieldField<fvsPatchField, scalar>& phi
) const
{
    mrfPtr_->makeRelative(phi);
}


void CML::fv::MRFSource::makeRelative
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    mrfPtr_->makeRelative(rho, phi);
}


void CML::fv::MRFSource::makeAbsolute(surfaceScalarField& phi) const
{
    mrfPtr_->makeAbsolute(phi);
}


void CML::fv::MRFSource::makeAbsolute
(
    const surfaceScalarField& rho,
    surfaceScalarField& phi
) const
{
    mrfPtr_->makeAbsolute(rho, phi);
}


void CML::fv::MRFSource::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);
}


bool CML::fv::MRFSource::read(const dictionary& dict)
{
    if (option::read(dict))
    {
        coeffs_.readIfPresent("UName", UName_);

        initialise();

        return true;
    }
    else
    {
        return false;
    }
}

