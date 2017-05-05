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

#include "fvPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvBoundaryMesh.hpp"
#include "fvMesh.hpp"
#include "primitiveMesh.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(fvPatch, 0);
    defineRunTimeSelectionTable(fvPatch, polyPatch);
    addToRunTimeSelectionTable(fvPatch, fvPatch, polyPatch);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fvPatch::fvPatch(const polyPatch& p, const fvBoundaryMesh& bm)
:
    polyPatch_(p),
    boundaryMesh_(bm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fvPatch::~fvPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::fvPatch::constraintType(const word& pt)
{
    return fvPatchField<scalar>::patchConstructorTablePtr_->found(pt);
}


CML::wordList CML::fvPatch::constraintTypes()
{
    wordList cTypes(polyPatchConstructorTablePtr_->size());

    label i = 0;

    for
    (
        polyPatchConstructorTable::iterator cstrIter =
            polyPatchConstructorTablePtr_->begin();
        cstrIter != polyPatchConstructorTablePtr_->end();
        ++cstrIter
    )
    {
        if (constraintType(cstrIter.key()))
        {
            cTypes[i++] = cstrIter.key();
        }
    }

    cTypes.setSize(i);

    return cTypes;
}


const CML::labelUList& CML::fvPatch::faceCells() const
{
    return polyPatch_.faceCells();
}


const CML::vectorField& CML::fvPatch::Cf() const
{
    return boundaryMesh().mesh().Cf().boundaryField()[index()];
}


CML::tmp<CML::vectorField> CML::fvPatch::Cn() const
{
    tmp<vectorField> tcc(new vectorField(size()));
    vectorField& cc = tcc();

    const labelUList& faceCells = this->faceCells();

    // get reference to global cell centres
    const vectorField& gcc = boundaryMesh().mesh().cellCentres();

    forAll(faceCells, faceI)
    {
        cc[faceI] = gcc[faceCells[faceI]];
    }

    return tcc;
}


CML::tmp<CML::vectorField> CML::fvPatch::nf() const
{
    return Sf()/magSf();
}


const CML::vectorField& CML::fvPatch::Sf() const
{
    return boundaryMesh().mesh().Sf().boundaryField()[index()];
}


const CML::scalarField& CML::fvPatch::magSf() const
{
    return boundaryMesh().mesh().magSf().boundaryField()[index()];
}


CML::tmp<CML::vectorField> CML::fvPatch::delta() const
{
    // Use patch-normal delta for all non-coupled BCs
    const vectorField nHat(nf());
    return nHat*(nHat & (Cf() - Cn()));
}


void CML::fvPatch::makeWeights(scalarField& w) const
{
    w = 1.0;
}


void CML::fvPatch::initMovePoints()
{}


void CML::fvPatch::movePoints()
{}


const CML::scalarField& CML::fvPatch::deltaCoeffs() const
{
    return boundaryMesh().mesh().deltaCoeffs().boundaryField()[index()];
}


const CML::scalarField& CML::fvPatch::weights() const
{
    return boundaryMesh().mesh().weights().boundaryField()[index()];
}


// ************************************************************************* //
