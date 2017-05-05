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

#include "sampledSurface.hpp"
#include "polyMesh.hpp"
#include "demandDrivenData.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(sampledSurface, 0);
    defineRunTimeSelectionTable(sampledSurface, word);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sampledSurface::clearGeom() const
{
    deleteDemandDrivenData(SfPtr_);
    deleteDemandDrivenData(magSfPtr_);
    deleteDemandDrivenData(CfPtr_);
    area_ = -1;
}


void CML::sampledSurface::makeSf() const
{
    // It is an error to recalculate if the pointer is already set
    if (SfPtr_)
    {
        FatalErrorIn("CML::sampledSurface::makeSf()")
            << "face area vectors already exist"
            << abort(FatalError);
    }

    const faceList& theFaces = faces();
    SfPtr_ = new vectorField(theFaces.size());

    vectorField& values = *SfPtr_;
    forAll(theFaces, faceI)
    {
        values[faceI] = theFaces[faceI].normal(points());
    }
}


void CML::sampledSurface::makeMagSf() const
{
    // It is an error to recalculate if the pointer is already set
    if (magSfPtr_)
    {
        FatalErrorIn("CML::sampledSurface::makeMagSf()")
            << "mag face areas already exist"
            << abort(FatalError);
    }

    const faceList& theFaces = faces();
    magSfPtr_ = new scalarField(theFaces.size());

    scalarField& values = *magSfPtr_;
    forAll(theFaces, faceI)
    {
        values[faceI] = theFaces[faceI].mag(points());
    }
}


void CML::sampledSurface::makeCf() const
{
    // It is an error to recalculate if the pointer is already set
    if (CfPtr_)
    {
        FatalErrorIn("CML::sampledSurface::makeCf()")
            << "face centres already exist"
            << abort(FatalError);
    }

    const faceList& theFaces = faces();
    CfPtr_ = new vectorField(theFaces.size());

    vectorField& values = *CfPtr_;
    forAll(theFaces, faceI)
    {
        values[faceI] = theFaces[faceI].centre(points());
    }
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

CML::autoPtr<CML::sampledSurface> CML::sampledSurface::New
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
{
    const word sampleType(dict.lookup("type"));

    if (debug)
    {
        Info<< "Selecting sampledType " << sampleType << endl;
    }

    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(sampleType);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "sampledSurface::New"
            "(const word&, const polyMesh&, const dictionary&)"
        )   << "Unknown sample type "
            << sampleType << nl << nl
            << "Valid sample types : " << endl
            << wordConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<sampledSurface>(cstrIter()(name, mesh, dict));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sampledSurface::sampledSurface
(
    const word& name,
    const polyMesh& mesh
)
:
    name_(name),
    mesh_(mesh),
    interpolate_(false),
    SfPtr_(NULL),
    magSfPtr_(NULL),
    CfPtr_(NULL),
    area_(-1)
{}


CML::sampledSurface::sampledSurface
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
:
    name_(name),
    mesh_(mesh),
    interpolate_(dict.lookupOrDefault("interpolate", false)),
    SfPtr_(NULL),
    magSfPtr_(NULL),
    CfPtr_(NULL),
    area_(-1)
{
    dict.readIfPresent("name", name_);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sampledSurface::~sampledSurface()
{
    clearGeom();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::vectorField& CML::sampledSurface::Sf() const
{
    if (!SfPtr_)
    {
        makeSf();
    }

    return *SfPtr_;
}


const CML::scalarField& CML::sampledSurface::magSf() const
{
    if (!magSfPtr_)
    {
        makeMagSf();
    }

    return *magSfPtr_;
}


const CML::vectorField& CML::sampledSurface::Cf() const
{
    if (!CfPtr_)
    {
        makeCf();
    }

    return *CfPtr_;
}


CML::scalar CML::sampledSurface::area() const
{
    if (area_ < 0)
    {
        area_ = sum(magSf());
        reduce(area_, sumOp<scalar>());
    }

    return area_;
}

CML::tmp<CML::scalarField> CML::sampledSurface::sample
(
    const surfaceScalarField& sField
) const
{
    notImplemented("tmp<CML::scalarField> sampledSurface::sample");
    return tmp<scalarField>(NULL);
}


CML::tmp<CML::vectorField> CML::sampledSurface::sample
(
    const surfaceVectorField& sField
) const
{
    notImplemented("tmp<CML::vectorField> sampledSurface::sample");
    return tmp<vectorField>(NULL);
}

CML::tmp<CML::sphericalTensorField> CML::sampledSurface::sample
(
    const surfaceSphericalTensorField& sField
) const
{
    notImplemented("tmp<CML::sphericalTensorField> sampledSurface::sample");
    return tmp<sphericalTensorField>(NULL);
}


CML::tmp<CML::symmTensorField> CML::sampledSurface::sample
(
    const surfaceSymmTensorField& sField
) const
{
    notImplemented("tmp<CML::symmTensorField> sampledSurface::sample");
    return tmp<symmTensorField>(NULL);
}


CML::tmp<CML::tensorField> CML::sampledSurface::sample
(
    const surfaceTensorField& sField
) const
{
    notImplemented("tmp<CML::tensorField> sampledSurface::sample");
    return tmp<tensorField>(NULL);
}


CML::tmp<CML::Field<CML::scalar> >
CML::sampledSurface::project(const Field<scalar>& field) const
{
    tmp<Field<scalar> > tRes(new Field<scalar>(faces().size()));
    Field<scalar>& res = tRes();

    forAll(faces(), faceI)
    {
        res[faceI] = field[faceI];
    }

    return tRes;
}


CML::tmp<CML::Field<CML::scalar> >
CML::sampledSurface::project(const Field<vector>& field) const
{
    tmp<Field<scalar> > tRes(new Field<scalar>(faces().size()));
    project(tRes(), field);
    return tRes;
}


CML::tmp<CML::Field<CML::vector> >
CML::sampledSurface::project(const Field<sphericalTensor>& field) const
{
    tmp<Field<vector> > tRes(new Field<vector>(faces().size()));
    project(tRes(), field);
    return tRes;
}


CML::tmp<CML::Field<CML::vector> >
CML::sampledSurface::project(const Field<symmTensor>& field) const
{
    tmp<Field<vector> > tRes(new Field<vector>(faces().size()));
    project(tRes(), field);
    return tRes;
}


CML::tmp<CML::Field<CML::vector> >
CML::sampledSurface::project(const Field<tensor>& field) const
{
    tmp<Field<vector> > tRes(new Field<vector>(faces().size()));
    project(tRes(), field);
    return tRes;
}


void CML::sampledSurface::print(Ostream& os) const
{
    os << type();
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream &os, const sampledSurface& s)
{
    s.print(os);
    os.check("Ostream& operator<<(Ostream&, const sampledSurface&");
    return os;
}


// ************************************************************************* //
