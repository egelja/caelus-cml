/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "polyPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "polyBoundaryMesh.hpp"
#include "polyMesh.hpp"
#include "primitiveMesh.hpp"
#include "SubField.hpp"
#include "entry.hpp"
#include "dictionary.hpp"
#include "pointPatchField.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(polyPatch, 0);

    int polyPatch::disallowGenericPolyPatch
    (
        debug::debugSwitch("disallowGenericPolyPatch", 0)
    );

    defineRunTimeSelectionTable(polyPatch, word);
    defineRunTimeSelectionTable(polyPatch, dictionary);

    addToRunTimeSelectionTable(polyPatch, polyPatch, word);
    addToRunTimeSelectionTable(polyPatch, polyPatch, dictionary);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::polyPatch::movePoints(PstreamBuffers&, const pointField& p)
{
    primitivePatch::movePoints(p);
}

void CML::polyPatch::updateMesh(PstreamBuffers&)
{
    primitivePatch::clearGeom();
    clearAddressing();
}


void CML::polyPatch::clearGeom()
{
    primitivePatch::clearGeom();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::polyPatch::polyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm,
    const word& patchType
)
:
    patchIdentifier(name, index),
    primitivePatch
    (
        faceSubList(bm.mesh().faces(), size, start),
        bm.mesh().points()
    ),
    start_(start),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


CML::polyPatch::polyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm,
    const word& patchType
)
:
    patchIdentifier(name, dict, index),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().faces(),
            readLabel(dict.lookup("nFaces")),
            readLabel(dict.lookup("startFace"))
        ),
        bm.mesh().points()
    ),
    start_(readLabel(dict.lookup("startFace"))),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


CML::polyPatch::polyPatch
(
    const polyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    patchIdentifier(pp),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().faces(),
            pp.size(),
            pp.start()
        ),
        bm.mesh().points()
    ),
    start_(pp.start()),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


CML::polyPatch::polyPatch
(
    const polyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    patchIdentifier(pp, index),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().faces(),
            newSize,
            newStart
        ),
        bm.mesh().points()
    ),
    start_(newStart),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


CML::polyPatch::polyPatch
(
    const polyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const labelUList& mapAddressing,
    const label newStart
)
:
    patchIdentifier(pp, index),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().faces(),
            mapAddressing.size(),
            newStart
        ),
        bm.mesh().points()
    ),
    start_(newStart),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


CML::polyPatch::polyPatch(const polyPatch& p)
:
    patchIdentifier(p),
    primitivePatch(p),
    start_(p.start_),
    boundaryMesh_(p.boundaryMesh_),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::polyPatch::~polyPatch()
{
    clearAddressing();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::polyPatch::constraintType(const word& pt)
{
    return pointPatchField<scalar>::pointPatchConstructorTablePtr_->found(pt);
}


CML::wordList CML::polyPatch::constraintTypes()
{
    wordList cTypes(dictionaryConstructorTablePtr_->size());

    label i = 0;

    for
    (
        dictionaryConstructorTable::iterator cstrIter =
            dictionaryConstructorTablePtr_->begin();
        cstrIter != dictionaryConstructorTablePtr_->end();
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


const CML::polyBoundaryMesh& CML::polyPatch::boundaryMesh() const
{
    return boundaryMesh_;
}


const CML::vectorField::subField CML::polyPatch::faceCentres() const
{
    return patchSlice(boundaryMesh().mesh().faceCentres());
}


const CML::vectorField::subField CML::polyPatch::faceAreas() const
{
    return patchSlice(boundaryMesh().mesh().faceAreas());
}


// Return the patch face neighbour cell centres
CML::tmp<CML::vectorField> CML::polyPatch::faceCellCentres() const
{
    tmp<vectorField> tcc(new vectorField(size()));
    vectorField& cc = tcc();

    // get reference to global cell centres
    const vectorField& gcc = boundaryMesh_.mesh().cellCentres();

    const labelUList& faceCells = this->faceCells();

    forAll(faceCells, facei)
    {
        cc[facei] = gcc[faceCells[facei]];
    }

    return tcc;
}


CML::tmp<CML::scalarField> CML::polyPatch::areaFraction() const
{
    tmp<scalarField> tfraction(new scalarField(size()));
    scalarField& fraction = tfraction();

    const vectorField::subField faceAreas = this->faceAreas();
    const pointField& points = this->points();

    forAll(*this, facei)
    {
        const face& curFace = this->operator[](facei);
        fraction[facei] =
            mag(faceAreas[facei])/(curFace.mag(points) + ROOTVSMALL);
    }

    return tfraction;
}


const CML::labelUList& CML::polyPatch::faceCells() const
{
    if (!faceCellsPtr_)
    {
        faceCellsPtr_ = new labelList::subList
        (
            patchSlice(boundaryMesh().mesh().faceOwner())
        );
    }

    return *faceCellsPtr_;
}


const CML::labelList& CML::polyPatch::meshEdges() const
{
    if (!mePtr_)
    {
        mePtr_ =
            new labelList
            (
                primitivePatch::meshEdges
                (
                    boundaryMesh().mesh().edges(),
                    boundaryMesh().mesh().pointEdges()
                )
            );
    }

    return *mePtr_;
}


void CML::polyPatch::clearAddressing()
{
    primitivePatch::clearTopology();
    primitivePatch::clearPatchMeshAddr();
    deleteDemandDrivenData(faceCellsPtr_);
    deleteDemandDrivenData(mePtr_);
}


void CML::polyPatch::write(Ostream& os) const
{
    os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
    patchIdentifier::write(os);
    os.writeKeyword("nFaces") << size() << token::END_STATEMENT << nl;
    os.writeKeyword("startFace") << start() << token::END_STATEMENT << nl;
}


void CML::polyPatch::initOrder(PstreamBuffers&, const primitivePatch&) const
{}


bool CML::polyPatch::order
(
    PstreamBuffers&,
    const primitivePatch&,
    labelList& faceMap,
    labelList& rotation
) const
{
    // Nothing changed.
    return false;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::polyPatch::operator=(const polyPatch& p)
{
    clearAddressing();

    patchIdentifier::operator=(p);
    primitivePatch::operator=(p);
    start_ = p.start_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const polyPatch& p)
{
    p.write(os);
    os.check("Ostream& operator<<(Ostream& os, const polyPatch& p");
    return os;
}


// ************************************************************************* //
