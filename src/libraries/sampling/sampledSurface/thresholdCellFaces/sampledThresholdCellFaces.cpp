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

#include "sampledThresholdCellFaces.hpp"

#include "dictionary.hpp"
#include "volFields.hpp"
#include "volPointInterpolation.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvMesh.hpp"
#include "thresholdCellFaces.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(sampledThresholdCellFaces, 0);
    addNamedToRunTimeSelectionTable
    (
        sampledSurface,
        sampledThresholdCellFaces,
        word,
        thresholdCellFaces
    );
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool CML::sampledThresholdCellFaces::updateGeometry() const
{
    const fvMesh& fvm = static_cast<const fvMesh&>(mesh());

    // no update needed
    if (fvm.time().timeIndex() == prevTimeIndex_)
    {
        return false;
    }

    prevTimeIndex_ = fvm.time().timeIndex();

    // Optionally read volScalarField
    autoPtr<volScalarField> readFieldPtr_;

    // 1. see if field in database
    // 2. see if field can be read
    const volScalarField* cellFldPtr = nullptr;
    if (fvm.foundObject<volScalarField>(fieldName_))
    {
        if (debug)
        {
            Info<< "sampledThresholdCellFaces::updateGeometry() : lookup "
                << fieldName_ << endl;
        }

        cellFldPtr = &fvm.lookupObject<volScalarField>(fieldName_);
    }
    else
    {
        // Bit of a hack. Read field and store.

        if (debug)
        {
            Info<< "sampledThresholdCellFaces::updateGeometry() : reading "
                << fieldName_ << " from time " << fvm.time().timeName()
                << endl;
        }

        readFieldPtr_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    fieldName_,
                    fvm.time().timeName(),
                    fvm,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                fvm
            )
        );

        cellFldPtr = readFieldPtr_.operator->();
    }
    const volScalarField& cellFld = *cellFldPtr;


    thresholdCellFaces surf
    (
        fvm,
        cellFld.internalField(),
        lowerThreshold_,
        upperThreshold_,
        triangulate_
    );

    const_cast<sampledThresholdCellFaces&>
    (
        *this
    ).MeshedSurface<face>::transfer(surf);
    meshCells_.transfer(surf.meshCells());

    // clear derived data
    sampledSurface::clearGeom();

    if (debug)
    {
        Pout<< "sampledThresholdCellFaces::updateGeometry() : constructed"
            << nl
            << "    field         : " << fieldName_ << nl
            << "    lowerLimit    : " << lowerThreshold_ << nl
            << "    upperLimit    : " << upperThreshold_ << nl
            << "    point         : " << points().size() << nl
            << "    faces         : " << faces().size() << nl
            << "    cut cells     : " << meshCells_.size() << endl;
    }

    return true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sampledThresholdCellFaces::sampledThresholdCellFaces
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
:
    sampledSurface(name, mesh, dict),
    fieldName_(dict.lookup("field")),
    lowerThreshold_(dict.lookupOrDefault<scalar>("lowerLimit", -VGREAT)),
    upperThreshold_(dict.lookupOrDefault<scalar>("upperLimit", VGREAT)),
    zoneKey_(keyType::null),
    triangulate_(dict.lookupOrDefault("triangulate", false)),
    prevTimeIndex_(-1),
    meshCells_(0)
{
    if (!dict.found("lowerLimit") && !dict.found("upperLimit"))
    {
        FatalErrorInFunction
            << "require at least one of 'lowerLimit' or 'upperLimit'" << endl
            << abort(FatalError);
    }

//    dict.readIfPresent("zone", zoneKey_);
//
//    if (debug && zoneKey_.size() && mesh.cellZones().findZoneID(zoneKey_) < 0)
//    {
//        Info<< "cellZone " << zoneKey_
//            << " not found - using entire mesh" << endl;
//    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sampledThresholdCellFaces::~sampledThresholdCellFaces()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::sampledThresholdCellFaces::needsUpdate() const
{
    const fvMesh& fvm = static_cast<const fvMesh&>(mesh());

    return fvm.time().timeIndex() != prevTimeIndex_;
}


bool CML::sampledThresholdCellFaces::expire()
{
    // already marked as expired
    if (prevTimeIndex_ == -1)
    {
        return false;
    }

    // force update
    prevTimeIndex_ = -1;
    return true;
}


bool CML::sampledThresholdCellFaces::update()
{
    return updateGeometry();
}


CML::tmp<CML::scalarField> CML::sampledThresholdCellFaces::sample
(
    const volScalarField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::vectorField> CML::sampledThresholdCellFaces::sample
(
    const volVectorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::sphericalTensorField> CML::sampledThresholdCellFaces::sample
(
    const volSphericalTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::symmTensorField> CML::sampledThresholdCellFaces::sample
(
    const volSymmTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::tensorField> CML::sampledThresholdCellFaces::sample
(
    const volTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::scalarField> CML::sampledThresholdCellFaces::interpolate
(
    const interpolation<scalar>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::vectorField> CML::sampledThresholdCellFaces::interpolate
(
    const interpolation<vector>& interpolator
) const
{
    return interpolateField(interpolator);
}

CML::tmp<CML::sphericalTensorField>
CML::sampledThresholdCellFaces::interpolate
(
    const interpolation<sphericalTensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::symmTensorField> CML::sampledThresholdCellFaces::interpolate
(
    const interpolation<symmTensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::tensorField> CML::sampledThresholdCellFaces::interpolate
(
    const interpolation<tensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


void CML::sampledThresholdCellFaces::print(Ostream& os) const
{
    os  << "sampledThresholdCellFaces: " << name() << " :"
        << "  field:" << fieldName_
        << "  lowerLimit:" << lowerThreshold_
        << "  upperLimit:" << upperThreshold_;
        //<< "  faces:" << faces().size()   // possibly no geom yet
        //<< "  points:" << points().size();
}


// ************************************************************************* //
