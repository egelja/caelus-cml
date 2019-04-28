/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "localAxesRotation.hpp"
#include "axesRotation.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "polyMesh.hpp"
#include "tensorIOField.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(localAxesRotation, 0);
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        localAxesRotation,
        dictionary
    );
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        localAxesRotation,
        objectRegistry
    );
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

void CML::localAxesRotation::init
(
    const objectRegistry& obr,
    const List<label>& cells
)
{
    const polyMesh& mesh = refCast<const polyMesh>(obr);
    const vectorField& cc = mesh.cellCentres();

    if (cells.size())
    {
        Rptr_.reset(new tensorField(cells.size()));

        tensorField& R = Rptr_();
        forAll(cells, i)
        {
            label cellI = cells[i];
            vector dir = cc[cellI] - origin_;
            dir /= mag(dir) + VSMALL;

            R[i] = axesRotation(e3_, dir).R();
        }
    }
    else
    {
        Rptr_.reset(new tensorField(mesh.nCells()));

        tensorField& R = Rptr_();
        forAll(cc, cellI)
        {
            vector dir = cc[cellI] - origin_;
            dir /= mag(dir) + VSMALL;

            R[cellI] = axesRotation(e3_, dir).R();
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::localAxesRotation::localAxesRotation
(
    const dictionary& dict,
    const objectRegistry& obr
)
:
    Rptr_(),
    origin_(point::zero),
    e3_(vector::zero)
{
    // If origin is specified in the coordinateSystem
    if (dict.parent().found("origin"))
    {
        dict.parent().lookup("origin") >> origin_;
    }

    // rotation axis
    dict.lookup("e3") >> e3_;

    init(obr);
}


CML::localAxesRotation::localAxesRotation
(
    const objectRegistry& obr,
    const vector& axis,
    const point& origin
)
:
    Rptr_(),
    origin_(origin),
    e3_(axis)
{
    init(obr);
}


CML::localAxesRotation::localAxesRotation
(
    const objectRegistry& obr,
    const vector& axis,
    const point& origin,
    const List<label>& cells
)
:
    Rptr_(),
    origin_(origin),
    e3_(axis)
{
    init(obr, cells);
}


CML::localAxesRotation::localAxesRotation(const dictionary& dict)
:
    Rptr_(),
    origin_(),
    e3_()
{
    FatalErrorInFunction
        << " localAxesRotation can not be constructed from dictionary "
        << " use the construtctor : "
           "("
           "    const dictionary&, const objectRegistry&"
           ")"
        << exit(FatalIOError);
}


CML::localAxesRotation::localAxesRotation(const tensorField& R)
:
    Rptr_(),
    origin_(vector::zero),
    e3_(vector::zero)
{
    Rptr_() = R;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::localAxesRotation::clear()
{
    if (!Rptr_.empty())
    {
        Rptr_.clear();
    }
}


void CML::localAxesRotation::updateCells
(
    const polyMesh& mesh,
    const labelList& cells
)
{
    const vectorField& cc = mesh.cellCentres();
    tensorField& R = Rptr_();

    forAll(cells, i)
    {
        label cellI = cells[i];
        vector dir = cc[cellI] - origin_;
        dir /= mag(dir) + VSMALL;

        R[cellI] = axesRotation(e3_, dir).R();
    }
}


CML::tmp<CML::vectorField> CML::localAxesRotation::transform
(
    const vectorField& vf
) const
{
    if (Rptr_->size() != vf.size())
    {
        FatalErrorInFunction
            << "vectorField st has different size to tensorField "
            << abort(FatalError);
    }

    return (Rptr_() & vf);
}


CML::vector CML::localAxesRotation::transform(const vector& v) const
{
    NotImplemented;
    return vector::zero;
}


CML::vector CML::localAxesRotation::transform
(
    const vector& v,
    const label cmptI
) const
{
    return (Rptr_()[cmptI] & v);
}


CML::tmp<CML::vectorField> CML::localAxesRotation::invTransform
(
    const vectorField& vf
) const
{
    return (Rptr_().T() & vf);
}


CML::vector CML::localAxesRotation::invTransform(const vector& v) const
{
    NotImplemented;
    return vector::zero;
}


CML::vector CML::localAxesRotation::invTransform
(
    const vector& v,
    const label cmptI
) const
{
    return (Rptr_()[cmptI].T() & v);
}


CML::tmp<CML::tensorField> CML::localAxesRotation::transformTensor
(
    const tensorField& tf
) const
{
    if (Rptr_->size() != tf.size())
    {
        FatalErrorInFunction
            << "tensorField st has different size to tensorField Tr"
            << abort(FatalError);
    }
    return (Rptr_() & tf & Rptr_().T());
}


CML::tensor CML::localAxesRotation::transformTensor
(
    const tensor& t
) const
{
    NotImplemented;

    return tensor::zero;
}


CML::tmp<CML::tensorField> CML::localAxesRotation::transformTensor
(
    const tensorField& tf,
    const labelList& cellMap
) const
{
    if (cellMap.size() != tf.size())
    {
        FatalErrorInFunction
            << "tensorField tf has different size to tensorField Tr"
            << abort(FatalError);
    }

    const tensorField& R = Rptr_();
    const tensorField Rtr(R.T());
    tmp<tensorField> tt(new tensorField(cellMap.size()));
    tensorField& t = tt();
    forAll(cellMap, i)
    {
        const label cellI = cellMap[i];
        t[i] = R[cellI] & tf[i] & Rtr[cellI];
    }

    return tt;
}


CML::tmp<CML::symmTensorField> CML::localAxesRotation::transformVector
(
    const vectorField& vf
) const
{
    if (Rptr_->size() != vf.size())
    {
        FatalErrorInFunction
            << "tensorField vf has different size to tensorField Tr"
            << abort(FatalError);
    }

    tmp<symmTensorField> tfld(new symmTensorField(Rptr_->size()));
    symmTensorField& fld = tfld();

    const tensorField& R = Rptr_();
    forAll(fld, i)
    {
        fld[i] = transformPrincipal(R[i], vf[i]);
    }
    return tfld;
}


CML::symmTensor CML::localAxesRotation::transformVector
(
    const vector& v
) const
{
    NotImplemented;
    return symmTensor::zero;
}


void CML::localAxesRotation::write(Ostream& os) const
{
     os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}


// ************************************************************************* //
