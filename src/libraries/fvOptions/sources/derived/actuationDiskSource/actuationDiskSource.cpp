/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "actuationDiskSource.hpp"
#include "fvMesh.hpp"
#include "fvMatrix.hpp"
#include "geometricOneField.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{
    defineTypeNameAndDebug(actuationDiskSource, 0);
    addToRunTimeSelectionTable
    (
        option,
        actuationDiskSource,
        dictionary
    );
}
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::fv::actuationDiskSource::checkData() const
{
    if (magSqr(diskArea_) <= VSMALL)
    {
        FatalErrorInFunction
           << "diskArea is approximately zero"
           << exit(FatalIOError);
    }
    if (Cp_ <= VSMALL || Ct_ <= VSMALL)
    {
        FatalErrorInFunction
           << "Cp and Ct must be greater than zero"
           << exit(FatalIOError);
    }
    if (mag(diskDir_) < VSMALL)
    {
        FatalErrorInFunction
           << "disk direction vector is approximately zero"
           << exit(FatalIOError);
    }
    if (returnReduce(upstreamCellId_, maxOp<label>()) == -1)
    {
        FatalErrorInFunction
           << "upstream location " << upstreamPoint_  << " not found in mesh"
           << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fv::actuationDiskSource::actuationDiskSource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(name, modelType, dict, mesh),
    diskDir_(coeffs_.lookup("diskDir")),
    Cp_(readScalar(coeffs_.lookup("Cp"))),
    Ct_(readScalar(coeffs_.lookup("Ct"))),
    diskArea_(readScalar(coeffs_.lookup("diskArea"))),
    upstreamPoint_(coeffs_.lookup("upstreamPoint")),
    upstreamCellId_(-1)
{
    coeffs_.lookup("fieldNames") >> fieldNames_;
    applied_.setSize(fieldNames_.size(), false);

    Info<< "    - creating actuation disk zone: "
        << this->name() << endl;

    upstreamCellId_ = mesh.findCell(upstreamPoint_);

    checkData();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fv::actuationDiskSource::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    const scalarField& cellsV = mesh_.V();
    vectorField& Usource = eqn.source();
    const vectorField& U = eqn.psi();

    if (V() > VSMALL)
    {
        addActuationDiskAxialInertialResistance
        (
            Usource,
            cells_,
            cellsV,
            geometricOneField(),
            U
        );
    }
}


void CML::fv::actuationDiskSource::addSup
(
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    const scalarField& cellsV = mesh_.V();
    vectorField& Usource = eqn.source();
    const vectorField& U = eqn.psi();

    if (V() > VSMALL)
    {
        addActuationDiskAxialInertialResistance
        (
            Usource,
            cells_,
            cellsV,
            rho,
            U
        );
    }
}


void CML::fv::actuationDiskSource::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);
}


bool CML::fv::actuationDiskSource::read(const dictionary& dict)
{
    if (option::read(dict))
    {
        coeffs_.readIfPresent("diskDir", diskDir_);
        coeffs_.readIfPresent("Cp", Cp_);
        coeffs_.readIfPresent("Ct", Ct_);
        coeffs_.readIfPresent("diskArea", diskArea_);

        checkData();

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
