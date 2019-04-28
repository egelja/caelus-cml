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

#include "explicitPorositySource.hpp"
#include "fvMesh.hpp"
#include "fvMatrices.hpp"
#include "porosityModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{
    defineTypeNameAndDebug(explicitPorositySource, 0);
    addToRunTimeSelectionTable
    (
        option,
        explicitPorositySource,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fv::explicitPorositySource::explicitPorositySource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(name, modelType, dict, mesh),
    porosityPtr_(nullptr)
{
    read(dict);

    if (selectionMode_ != smCellZone)
    {
        FatalErrorInFunction
            << "The porosity region must be specified as a cellZone.  Current "
            << "selection mode is " << selectionModeTypeNames_[selectionMode_]
            << exit(FatalError);
    }

    porosityPtr_.reset
    (
        porosityModel::New
        (
            name_,
            mesh_,
            coeffs_,
            cellSetName_
        ).ptr()
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fv::explicitPorositySource::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    fvMatrix<vector> porosityEqn(eqn.psi(), eqn.dimensions());
    porosityPtr_->addResistance(porosityEqn);
    eqn -= porosityEqn;
}


void CML::fv::explicitPorositySource::addSup
(
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    fvMatrix<vector> porosityEqn(eqn.psi(), eqn.dimensions());
    porosityPtr_->addResistance(porosityEqn);
    eqn -= porosityEqn;
}


void CML::fv::explicitPorositySource::addSup
(
    const volScalarField& alpha,
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    fvMatrix<vector> porosityEqn(eqn.psi(), eqn.dimensions());
    porosityPtr_->addResistance(porosityEqn);
    eqn -= alpha*porosityEqn;
}


void CML::fv::explicitPorositySource::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);
}


bool CML::fv::explicitPorositySource::read(const dictionary& dict)
{
    if (option::read(dict))
    {
        if (coeffs_.found("UNames"))
        {
            coeffs_.lookup("UNames") >> fieldNames_;
        }
        else if (coeffs_.found("UName"))
        {
            word UName(coeffs_.lookup("UName"));
            fieldNames_ = wordList(1, UName);
        }
        else
        {
            fieldNames_ = wordList(1, "U");
        }

        applied_.setSize(fieldNames_.size(), false);

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
